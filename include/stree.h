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


void setdepthheadposition(STree *stree,Uint depth, Uint headposition);

void setsuffixlink(STree *stree,Uint slink);

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


#endif
