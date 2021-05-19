#include "example.h"

mh_map_t *commands;

MH_CONSTRUCTOR(200) void commands_ctr(void) {
    commands = mh_map_new(MH_GLOBAL);
}

typedef void command_t(void);

int main(int argc, char *argv[]) {
    static const mh_argument_parser_args_t args = {
            .optional_arguments = NULL,
            .required_arguments = "command"
    };
    mh_map_t *arguments = mh_argument_parse(MH_GLOBAL, &args, argc, argv);
    mh_memory_t command = mh_map_get(arguments, MH_STRING("command"));
    mh_ref_t command_func = mh_map_get(commands, command).address;
    if (command_func == NULL) {
        MH_WRITE_ERR("Invalid command `{}`.\n", MH_FMT_STR(command.address));
        exit(1);
    }
    ((command_t *) (size_t) command_func)();
}
