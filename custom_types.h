#pragma once
typedef std::string string;
typedef const std::string const_string;
typedef char Color;
typedef unsigned int counter_v;

#define ignore_test "*"
#define another_suit "$"

const_string SUCCESS = "No error";
const_string FAILURE = "Failure";
const_string SKIPPED = "Skipped";

const_string EXIT = "^C";
const_string HELP_short = "-h";
const_string HELP_long = "--help";
