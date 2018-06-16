/*
 * Copyright (c) 2003 by Stefan Kurtz and The Institute for
 * Genomic Research.  This is OSI Certified Open Source Software.
 * Please see the file LICENSE for licensing information and
 * the file ACKNOWLEDGEMENTS for names of contributors to the
 * code base.
 *
 * Modified by Ludvig Sundström 2018 under permission by Stefan Kurtz.
 */

#include "stree.h"

Wchar *text;
Wchar *sentinel;
Uint textlen, alphasize;


Bool tail_at_lastchar(STree *st)
{
    return st->tail == sentinel;
}


Bool head_is_root(STree *st)
{
    return st->head.depth == 0;
}

VertexP get_next_inner(STree *st)
{
    return st->is.fst + st->is.nxt_ind;
}


VertexP get_next_leaf(STree *st)
{
    return st->ls.fst + st->ls.nxt_ind;
}


Uint get_next_leafnum(STree *st)
{
    return st->ls.nxt_ind | LEAFBIT;
}


void set_next_leaf(STree *st, Vertex v)
{
    *st->ls.nxt = v;
}


void st_free(STree *st)
{
    free(st->ls.fst);
    free(st->rootchildren);
    free(st->is.fst);
}


