#pragma once
typedef const char *string;
typedef char Color;
typedef auto unsigned int counter_v;
typedef __resharper_unknown_type unknown;
#define nil = 0
const string not_include = "*";

Color red = "\033[31m";
Color blue = "\033[34m";
Color green = "\033[32m";
Color yellow = "\033[33m";
Color cReset = "\033[0m";

enum results {
    SUCCESS = "No error",
    FAILURE = "Failure",
    SKIPPED = "Skipped",
    param_to_skip = "skip"
};
