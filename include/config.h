#ifndef BASEDEF_H
#define BASEDEF_H

#define MAX_ALLOC           450000000 // 450 MB

#define BRANCHWIDTH             UintConst(2)

#define MAXPATTERNLEN           100000

#define MAX_CHARS               100000

#define MAX_PATTERNS            100000

#define LOGWORDSIZE    6

#define INTWORDSIZE\
        (UintConst(1) << LOGWORDSIZE)     // # of bits in Uint = w

#endif