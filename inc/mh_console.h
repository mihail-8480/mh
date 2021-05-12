#ifndef MH_MH_CONSOLE_H
#define MH_MH_CONSOLE_H

#include "mh_map.h"
#include "mh_stream.h"

/*
 * <mh_args.h>
 * The libmh console application header.
 *
 * Contains an argument parser.
 */

extern mh_stream_t *mh_console_output_stream;
extern mh_stream_t *mh_console_error_stream;

// The argument parser settings.
MH_API_TYPE(mh_argument_parser_args, struct mh_argument_parser_args {
    // Required arguments with a space in between.
    char *required_arguments;
    // Optional arguments that come after the required arguments.
    char *optional_arguments;
});

// Parse arguments and return a map filled with them.
MH_API_FUNC(mh_map_t *
            mh_argument_parse(
                    mh_context_t *context, const mh_argument_parser_args_t *args, int argc, char *argv[]));

#define MH_OUT mh_console_output_stream
#define MH_ERR mh_console_error_stream

#define MH_PRINT(c_str) mh_stream_write_reference(MH_OUT, c_str, sizeof(c_str)-1)
#define MH_PRINT_STR(str) mh_stream_write_reference(MH_OUT, str, strlen(str))
#define MH_PRINT_MEM(mem) mh_stream_write(MH_OUT, &mem, mem.size)
#define MH_PRINT_ERROR(c_str) mh_stream_write_reference(MH_ERR, c_str, sizeof(c_str)-1)
#define MH_PRINT_ERROR_STR(str) mh_stream_write_reference(MH_ERR, str, strlen(str))
#define MH_PRINT_ERROR_MEM(mem) mh_stream_write(MH_ERR, &mem, mem.size)

#endif //MH_MH_CONSOLE_H
