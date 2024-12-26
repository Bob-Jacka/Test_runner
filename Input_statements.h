#ifndef INPUT_STATEMENTS_H
#define INPUT_STATEMENTS_H
#include "custom_types.h"

struct statements {
    const_string yes = "yes";
    const_string no = "no";
    const_string one = "1";
    const_string zero = "0";
    const_string skip = "skip";

#ifdef WIN32 || WIN64
    const_string EXIT = "^C";
#endif

#ifdef LINUX
    const_string EXIT = "^D";
#endif

    const_string long_help = "--help";
    const_string short_help = "-h";

    const_string SUCCESS = "No error";
    const_string FAILURE = "Failure";
    const_string SKIPPED = "Skipped";
};

static statements input_statements;

#endif //INPUT_STATEMENTS_H
