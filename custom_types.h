#pragma once
typedef std::string string;
typedef const std::string const_string;
typedef char Color;
typedef unsigned int counter_v;

// #define  red  "\x1b[31m"
// #define   blue  "\x1b[34m"
// #define   green  "\x1b[32m"
// #define   yellow  "\x1b[33m"
// #define   cReset  "\x1b[0m"

#define ignore_test "*"
#define another_suit "$"

const_string SUCCESS = "No error";
const_string FAILURE = "Failure";
const_string SKIPPED = "Skipped";

const_string EXIT = "^C";
const_string HELP_short = "-h";
const_string HELP_long = "--help";
