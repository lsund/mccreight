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
    Uint next_num;
    Uint size;

} Table;


typedef struct text {
    Wchar *content;                // points to the input string
    Wchar *sentinel;               // points to the position of the \(\$\)-symbol
    Uint len;                      // the length of the input string
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
    VertexP vertex;           // left component of head location, that is u
} Head;


typedef struct suffixtree {

    // For each inner vertex, inner[w] stores a branch record consisting of
    // file components:
    // 1. Firstchild - the first child of w
    //
    // 2. branchsibling - the right sibling of w, or Nothing
    //
    // 3. depth - vertex depth of w. Storing the depth helps with edge
    // retrieval, and also has some practical advantages. (1) The depth of a
    // node never changes (2) the depth are decremented by one when following
    // suffix links, which can be explaited. (3) also, several applications of
    // suffix trees assume the depth is available.
    //
    // 4. headposition - the head position of w
    //
    // 5. suffixlink - the suffix link for w
    //
    // The successors of a inner vertex are therefore found in a list whose
    // elements are linked via firstchild, branchsibling and tleaf
    // references. To speed this up, this list is ordered according to the
    // first character.
    Table inner;
    // For each leaf number j, leafs store a reference to the right sibling of
    // the corresponding suffix. If there is no such sibling, then
    // leafs[j] = nothing
    Table leaves;

    Uint *rootchildren;         // references to successors of root

    Uint split_vertex;            // the node the split edge leads to
    Uint insertprev;            // the edge preceeding the split edge

    Uint currentdepth;          // depth of the new branch node
    Uint branchnodeoffset;      // number of leafs in tree
    Uint alphasize;             // the number of different characters in t
    Uint maxbranchdepth;        // maximal depth of branching node
    Uint *setlink;              // address of a nil-reference
    Chain chain;           // address of the node current chains starts at

    Head headedge;

    // refers to the last address, such that at
    // least LARGE_VERTEXSIZE integers are available. So a large node can be
    // stored in the available amount of space.
    Uint *allocated;

    // holds counts of the number of leafs in subtree indexed by headposition
    Uint *leafcounts;
    Bool is_nil_stored;          // nil-reference is stored in new leaf
    Wchar *tailptr;            // points to the tail

} STree;

#endif
