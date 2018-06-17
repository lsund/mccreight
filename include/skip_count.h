#ifndef SKIP_COUNT_H
#define SKIP_COUNT_H

#include "streedef.h"
#include "externs.h"
#include "chain.h"

// Find  the location of the current head by only looking at the fst
// character of each edge.
void skip_count(STree *st);

#endif
