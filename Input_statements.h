#ifndef INPUT_STATEMENTS_H
#define INPUT_STATEMENTS_H

// pass
#define FIRST_PASS_CHOICE "yes"
#define SECOND_PASS_CHOICE "1"

// no pass
#define FIRST_FAILURE_CHOICE "no"
#define SECOND_FAILURE_CHOICE "0"

// skip test
#define SKIP_CHOICE "skip"

// help flag
#define LONG_FLAG_CHOICE "--help"
#define SHORT_FLAG_CHOICE "-h"

//Test statements
#define TEST_SUCCESS "No error"
#define TEST_FAILURE "Failure"
#define TEST_SKIPPED "Skipped"

//Program settings
inline char *save_file_name = "save-point";
inline char *save_ext = ".txt";

// System defined exit symbol
#if defined(_WIN32) || defined(_WIN64)
    #define EXIT "^C"
    #define NEW_LINE "\r\n"
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    #define EXIT "^D"
    #define NEW_LINE "\n"
#endif

//Line in test suit file will be ignored if it starts with this symbol
#define IGNORE_TEST "*"

//Line in file will be interpreted like test suit with this symbol at start
#define ANOTHER_SUIT "$"

//Cursor that displays every string or int input
#define INPUT_CURSOR ">> "

//Message that displays before every test case
#define EVERY_TEST_MESSAGE "Enter (yes / 1) for success or (no / 0) for failure or skip to skip, -h for help"

#endif //INPUT_STATEMENTS_H
