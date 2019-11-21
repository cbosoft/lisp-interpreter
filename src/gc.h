#pragma once
#include <gc.h>

#define malloc(N) GC_MALLOC(N)
#define calloc(N,S) GC_MALLOC(N*S)
#define realloc(P,N) GC_REALLOC(P,N)
