#pragma once

/*
* inner array in game results representing one test result
 */
typedef struct {
    bool pass;
    string name;
    string errors_in_test;
} one_test_result;

/*
 * type that define game results
 */
typedef struct {
    string game_name;
    string platform_name;

    void (*clearState)();

    void (*init)();

    void (*size)();

    void (*forAnyOne)();

    one_test_result stages_res[];
} one_device_results;

one_test_result empty = {
    .pass = false,
    .name = "",
    .errors_in_test = ""
};

/*
 * method that makes unique object of game_result object
 */
inline one_device_results __init__(const string game_name, const string platform_name) {
    // const one_test_result test_result[];
    const one_device_results to_return = {
        .game_name = game_name,
        .platform_name = platform_name,
        // .stages_res = test_result
    };
    return to_return;
}

/*
 * method that clears array
 */
inline void clear(char **array, const unsigned int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

inline counter_v size(const one_device_results *result) {
    return sizeof(&result->stages_res) / sizeof(result->stages_res[0]);
}

/*
 * for each inner element print
 */
inline void foreach(const one_device_results iterable, unknown *func) {
    const counter_v length = size(&iterable);
    int (*fptr)(one_test_result) = func;
    for (counter_v i = 0; i < length; i++) {
        fptr(iterable.stages_res[i]); //TODO может быть ошибка из - за того, что не ссылка
    }
}

inline void enter_data(one_test_result to_input, const bool is_pass, const string test_name, const string errors) {
    to_input.pass = is_pass;
    to_input.name = test_name;
    to_input.errors_in_test = errors;
}
