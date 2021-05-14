#include "../../inc/mh_console.h"
#include "../streams/mh_stream_private.h"

mh_stream_t *mh_console_error_stream;
mh_stream_t *mh_console_output_stream;

mh_writer_t *mh_console_error;
mh_writer_t *mh_console_output;

MH_CONSTRUCTOR(102) void mh_init_streams(void) {
    mh_console_error_stream = mh_file_stream_new(MH_GLOBAL, stderr, false);
    mh_console_output_stream = mh_file_stream_new(MH_GLOBAL, stdout, false);
    mh_stream_private_t *private_err = ((mh_stream_private_t *) mh_console_error_stream);
    mh_stream_private_t *private_out = ((mh_stream_private_t *) mh_console_output_stream);
    private_err->can_read = false;
    private_err->read = NULL;
    private_out->can_read = false;
    private_out->read = NULL;
    mh_console_error = mh_writer_from_stream(mh_console_error_stream);
    mh_console_output = mh_writer_from_stream(mh_console_output_stream);
}

static bool mh_argument_parse_one(mh_map_t *map, int argc, char *argv[], mh_memory_t *format, int *c_arg) {
    while (*c_arg < argc) {
        if (format->offset >= format->size) {
            return true;
        }
        mh_memory_t current = mh_memory_read_until(format, ' ');
        if (current.size == 0) {
            current = mh_memory_reference((char *) format->address + format->offset, format->size - format->offset);
            format->offset = format->size;
        }
        mh_memory_t value = MH_REF_STRING(argv[*c_arg]);
        mh_map_set(map, current, value);
        *c_arg = *c_arg + 1;
    }
    return false;
}

mh_map_t *mh_argument_parse(mh_context_t *context, const mh_argument_parser_args_t *args, int argc, char *argv[]) {
    mh_map_t *map = mh_map_new(context);
    mh_memory_t req = MH_REF_STRING(args->required_arguments);

    int c_arg = 1;
    bool too_many_req = mh_argument_parse_one(map, argc, argv, &req, &c_arg);

    if (req.offset != req.size) {
        MH_THROW(context, "Not enough arguments.");
    }

    if (too_many_req && args->optional_arguments != NULL) {
        mh_memory_t opt = MH_REF_STRING(args->optional_arguments);
        bool too_many_opt = mh_argument_parse_one(map, argc, argv, &opt, &c_arg);
        if (too_many_opt) {
            MH_THROW(context, "Too many arguments.");
        }
    } else if (too_many_req) {
        MH_THROW(context, "Too many arguments.");
    }

    return map;
}
