#ifndef INPUT_STATEMENTS_H
#define INPUT_STATEMENTS_H
#include "custom_types.h"

struct statements {
    const_string yes = "yes";
    const_string no = "no";

    const_string one = "1";
    const_string zero = "0";

    const_string skip = "skip";

    const_string long_help = "--help";
    const_string short_help = "-h";

    const_string SUCCESS = "No error";
    const_string FAILURE = "Failure";
    const_string SKIPPED = "Skipped";

    char *save_file_name = "save-point";

#ifdef WIN32 || WIN64
    const_string EXIT = "^C";
#endif

#ifdef LINUX
        const_string EXIT = "^D";
#endif
};

static statements input_statements;
#define ignore_test "*" //Line in test suit file will be ignored if it starts with this symbol
#define another_suit "$" //Line in file will be interpreted like test suit with this symbol at start

#endif //INPUT_STATEMENTS_H
