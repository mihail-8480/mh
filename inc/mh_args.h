#ifndef MH_MH_ARGS_H
#define MH_MH_ARGS_H

#include "mh_map.h"

MH_API_TYPE(mh_argument_parser_args, struct mh_argument_parser_args {
    char *required_arguments;
    char *optional_arguments;
});

MH_API_FUNC(mh_map_t *
            mh_argument_parse(mh_context_t *context, const mh_argument_parser_args_t *args, int argc, char *argv[]));

#endif //MH_MH_ARGS_H
