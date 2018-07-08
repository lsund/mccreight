#include "location.h"

Text text;

void init_loc(Uint *v, Uint hd, Uint depth, Loc *loc)
{
    loc->nxt          = v;
    loc->string.start  = hd;
    loc->string.length = depth;
    loc->remain        = 0;
    loc->leafedge      = false;
}


void make_loc(STree *st, Uint leafnum, Uint plen, Loc *loc, Wchar *fst)
{
    loc->string.start  = leafnum;
    loc->string.length = plen;
    loc->prev          = st->is.fst;
    loc->edgelen       = text.len - leafnum + 1;
    loc->remain        = loc->edgelen - plen;
    loc->nxt          = st->ls.fst + leafnum;
    loc->fst         = fst;
}



void update_loc(Uint *nxt, Uint start, Uint plen, Wchar *fst, Uint depth, Uint edgelen, Loc *loc)
{
    loc->string.start  = start;
    loc->string.length = depth + plen;
    loc->prev          = loc->nxt;
    loc->edgelen       = edgelen;
    loc->remain        = loc->edgelen - plen;
    loc->fst         = fst;
    loc->nxt          = nxt;
}
