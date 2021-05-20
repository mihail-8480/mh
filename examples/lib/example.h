#ifndef MH_EXAMPLE_H
#define MH_EXAMPLE_H

#include "../../inc/mh_console.h"

extern mh_map_t *commands;

#define COMMAND(T) \
void (T)(void); \
MH_CONSTRUCTOR(201) void (T##_ctr)(void) { \
    mh_map_set(commands, MH_STRING(#T), MH_REF_CONST((mh_ref_t)(size_t)T)); \
} \
void (T)(void)


#endif //MH_EXAMPLE_H
