#ifndef STREEDATA_H
#define STREEDATA_H

#include "types.h"
#include "arraydef.h"

///////////////////////////////////////////////////////////////////////////////
// Types


typedef Uint Vertex;

typedef Uint *VertexP;

typedef struct table {
    Uint *first;
    Uint *next;
    Uint next_ind;
    Uint size;
} Table;


typedef struct text {
    Wchar *content;                // points to the input string
    Wchar *sentinel;               // points to the position of the \(\$\)-symbol
    Uint len;                      // the length of the input string
    Uint alphasize;                // the number of different characters in t
} Text;


typedef struct chain {
    VertexP first;
    Uint size;
} Chain;


// The edge that contains the end of head
//
// The right component of the head location uv.
// head_start refers to the first character of v
// headend to the last character. In case, v = empty
// headend = null
// \(\emph{headend}=\emph{NULL}\).

typedef struct head {
    Wchar *start;
    Wchar *end;
    Uint depth;
    VertexP origin;           // the vertex u
} Head;


typedef struct suffixtree {

    Uint *rootchildren;         // references to successors of root
    Table inner;
    Table leaves;

    Uint split_vertex;          // the node the split edge leads to
    Uint insertprev;            // the edge preceeding the split edge

    Uint currentdepth;          // depth of the new branch node
    Uint maxbranchdepth;        // maximal depth of branching node
    Uint *setlink;              // address of a nil-reference
    Chain chain;                // address of the node current chains starts at

    Head headedge;

    // refers to the last address, such that at
    // least LARGE_VERTEXSIZE integers are available. So a large node can be
    // stored in the available amount of space.
    Uint *allocated;

    Bool is_nil_stored;          // nil-reference is stored in new leaf
    Wchar *tailptr;            // points to the tail

} STree;

#endif
