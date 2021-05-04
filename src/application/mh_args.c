#include "../../inc/mh_args.h"


static bool mh_argument_parse_one(mh_map_t* map, int argc, char *argv[], mh_memory_t* format, int *c_arg) {
    while(*c_arg < argc) {
        if (format->offset >= format->size) {
            return true;
        }
        mh_memory_t current = mh_memory_read_until(format, ' ');
        if (current.size == 0) {
            current = mh_memory_reference((char*)format->address + format->offset, format->size - format->offset);
            format->offset = format->size;
        }
        mh_memory_t value = MH_REF_STRING(argv[*c_arg]);
        mh_map_add(map, current, value);
        *c_arg = *c_arg + 1;
    }
    return false;
}

mh_map_t* mh_argument_parse(mh_context_t* context, const mh_argument_parser_args_t* args, int argc, char *argv[]) {
    mh_map_t* map = mh_map_new(context);
    mh_memory_t req = MH_REF_STRING(args->required_arguments);

    int c_arg = 1;
    bool too_many_req = mh_argument_parse_one(map, argc, argv, &req, &c_arg);

    if (req.offset != req.size) {
        mh_context_error(context, "Not enough arguments.", MH_LOCATION_ANY());
        return map;
    }

    if (too_many_req && args->optional_arguments != NULL) {
        mh_memory_t opt = MH_REF_STRING(args->optional_arguments);
        bool too_many_opt = mh_argument_parse_one(map, argc, argv, &opt, &c_arg);
        if (too_many_opt) {
            mh_context_error(context, "Too many arguments.", MH_LOCATION_ANY());
        }
    } else if(too_many_req) {
        mh_context_error(context, "Too many arguments.", MH_LOCATION_ANY());
    }

    return map;
}
