/*
 * Copyright (c) 2003 by Stefan Kurtz and The Institute for
 * Genomic Research.  This is OSI Certified Open Source Software.
 * Please see the file LICENSE for licensing information and
 * the file ACKNOWLEDGEMENTS for names of contributors to the
 * code base.
 *
 * Modified by Ludvig Sundström 2018 under permission by Stefan Kurtz.
 */


#ifndef STREE_H
#define STREE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wchar.h>
#include <limits.h>
#include <stdbool.h>

#include "basedef.h"
#include "spaceman.h"
#include "clock.h"
#include "externs.h"
#include "types.h"
#include "arraydef.h"
#include "bitvector.h"
#include "stree_aux.h"

// A path in the suffix tree is stored with the `Pathinfo` struct
typedef struct
{
    Uint depth, headposition;
    Uint *ref;
} Pathinfo;

DECLAREARRAYSTRUCT(Pathinfo);

typedef struct
{
    Bool secondtime;
    ArrayUint stack;
} DFSstate;

///////////////////////////////////////////////////////////////////////////////
// Functions

void setdepthheadposition(STree *stree,Uint depth, Uint headposition);

void setsuffixlink(STree *stree,Uint slink);

Uint getlargelinkconstruction(STree *stree);

void init(STree *stree);

// Slow-scan
void scanprefix(STree *stree);

// skip-count
void rescan(STree *stree);

// Insert a large node
void completelarge(STree *stree);

// Insert a leaf edge from the current base vertex
void insertleaf(STree *stree);

// Insert inner veretx and split appropriate edges
void insertbranchnode(STree *stree);

// Construct the successor chain for the children of the root. This is done of
// the end of the algorithm in one sweep.
void linkrootchildren(STree *stree);

void freestree(STree *stree);

///////////////////////////////////////////////////////////////////////////////
// Macros

/*
   We use the least significant bit to discriminate references to leafs
   and branching nodes. Since base addresses are even, the unset least
   significant bit of a reference identifies a base address. For a leaf
   reference we shift the leaf number one to the right and
   set the least significant bit.
   */

#define LABEL_START(ST, O)        (ST)->text + (O)
#define IS_LAST(ST, C)          (C) == (ST)->sentinel

#define IS_LEAF(V)                 ((V) & LEAFBIT)
#define MAKELEAF(V)               ((V) | LEAFBIT)
#define MAKELARGE(V)              (V)
#define MAKELARGELEAF(V)          MAKELEAF(V)

#define LEAF_NUM(V)           ((V) & ~(LEAFBIT | SMALLBIT))
#define LEAF_REF(ST, V)    (ST)->inner_vertices.first + LEAF_NUM((V))
#define LEAF_VERTEX(ST, N) (ST)->leaf_vertices.first[(N)]

#define IS_NOTHING(P)                 ((P) & NILBIT)
#define UNDEFREFERENCE            (~((Uint) 0))
#define MAXTEXTLEN                ((MAXINDEX/((LARGEINTS+SMALLINTS)/2)) - 3)

#define CHILD(B)               ((*(B)) & MAXINDEX)
#define SIBLING(B)             (*((B)+1))
#define GETSUFFIXLINK(B)          getlargelinkconstruction(stree)
#define SETCHILD(B,VAL)           SETVAL(B,((*(B)) & SMALLBIT) | (VAL))
#define SETBROTHER(B,VAL)         SETVAL(B+1,VAL)

#define SETDISTANCE(B,VAL)        SETVAL(B+2,VAL);\
    SETVAL(B,(*(B)) | SMALLBIT)
#define SETDEPTHHEADPOS(DP,HP)    SETVAL(stree->inner_vertices.next_free+2,DP);\
    SETVAL(stree->inner_vertices.next_free+3,HP)

#define SETNEWCHILD(B,VAL)        SETVAL(B,VAL)
#define SETNEWCHILDBROTHER(CH,BR) SETVAL(stree->inner_vertices.next_free,CH);\
    SETVAL(stree->inner_vertices.next_free+1,BR)

#define SETSUFFIXLINK(SL)         SETVAL(stree->inner_vertices.next_free+4,SL)

#define SETLEAFBROTHER(B,VAL)     *(B) = (VAL)

#define MAKEBRANCHADDR(V)         (V)
#define SETBRANCHNODEOFFSET       /* nothing */


#define ROOT_CHILD(ST, C)   ((ST)->rootchildren[(Uint) (C)])
#define ROOT(ST)            ((ST)->inner_vertices.first)

#define IS_ROOT(ST, V)          ((ST)->inner_vertices.first == V)

#define IS_UNDEF(V)          ((V) == UNDEFREFERENCE)

// Is the location the root?
#define ROOTLOCATION(LOC)\
    (((LOC)->string.length == 0) ? True : False)

// Index of a branch and leaf relative to the first address
#define INDEX_INNER(ST,A)      ((Uint) ((A) - ROOT(ST)))
#define LEAFADDR2NUM(ST,A)    ((Uint) ((A) - (ST)->leaf_vertices.first))

#define SETVAL(E,VAL) *(E) = VAL

#define GETDEPTHAFTERHEADPOS(DP,PT) \
    if(stree->chainstart != NULL && (PT) >= stree->chainstart)\
{\
    DP = stree->currentdepth + distance;\
} else\
{\
    if(ISLARGE(*(PT)))\
    {\
        DP = GETDEPTH(PT);\
    } else\
    {\
        DP = GETDEPTH(largep) + distance;\
    }\
}

#define FOLLOWSUFFIXLINK\
    if(ISLARGE(*(stree->headnode)))\
{\
    stree->headnode = stree->inner_vertices.first + GETSUFFIXLINK(stree->headnode);\
} else\
{\
    stree->headnode += SMALLINTS;\
}\
stree->headnodedepth--

// Set the address for a nil-reference. In the case the reference is a new
// leaf, this is marked
#define RECALLNEWLEAFADDRESS(A)   stree->setlink = A;\
                                                   stree->setatnewleaf = True
#define RECALLLEAFADDRESS(A)      stree->setlink = A;\
                                                   stree->setatnewleaf = False

#endif
