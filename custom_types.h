#pragma once
typedef char *string;
typedef const char *const_string;
typedef char Color;
typedef unsigned int counter_v;
template<typename T>
T unknown;
#define nil = 0
#define  red  "\x1b[31m"
#define   blue  "\x1b[34m"
#define   green  "\x1b[32m"
#define   yellow  "\x1b[33m"
#define   cReset  "\x1b[0m"
#define ignore_test "*"

const string SUCCESS = "No error";
const string FAILURE = "Failure";
const string SKIPPED = "Skipped";
