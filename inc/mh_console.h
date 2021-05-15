#ifndef MH_MH_CONSOLE_H
#define MH_MH_CONSOLE_H

#include "mh_map.h"
#include "mh_stream.h"

/*
 * <mh_args.h>
 * The libmh console application header.
 */

// A mh_stream_t that is connected to stdout.
extern mh_stream_t *mh_console_output_stream;

// A mh_stream_t that is connected to stderr.
extern mh_stream_t *mh_console_error_stream;

// A mh_writer_t that's connected to mh_console_error_stream.
extern mh_writer_t *mh_console_error;

// A mh_writer_t that's connected to mh_console_output_stream.
extern mh_writer_t *mh_console_output;

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

// Get an environment variable, if it's not found return the default value.
MH_API_FUNC(const char *mh_env_default(const char *env, const char *def));

// An alias for mh_console_output.
#define MH_OUTPUT mh_console_output

// An alias for mh_console_error.
#define MH_ERROR mh_console_error

// Write something to MH_OUTPUT.
#define MH_WRITE(...) mh_write(MH_OUTPUT, __VA_ARGS__)

// Write something to MH_ERROR.
#define MH_WRITE_ERR(...) mh_write(MH_ERROR, __VA_ARGS__)

#endif //MH_MH_CONSOLE_H
