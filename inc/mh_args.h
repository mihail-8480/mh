#ifndef MH_MH_ARGS_H
#define MH_MH_ARGS_H

#include "mh_map.h"

/*
 * <mh_args.h>
 * The libmh argument parser header.
 *
 * Contains an argument parser.
 */

// The argument parser settings.
MH_API_TYPE(mh_argument_parser_args, struct mh_argument_parser_args {
    // Required arguments with a space in between.
    char *required_arguments;
    // Optional arguments that come after the required arguments.
    char *optional_arguments;
});

// Parse arguments and return a map filled with them.
MH_API_FUNC(mh_map_t *
            mh_argument_parse(mh_context_t *context, const mh_argument_parser_args_t *args, int argc, char *argv[]));

#endif //MH_MH_ARGS_H
