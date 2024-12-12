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
    one_test_result stages_res[];

    void (*clearState)();
    void (*init)();
    void (*size)();
    void (*forAnyOne)();
} one_device_results;

one_device_results __init__(string game_name, string platform_name, unsigned int len);

void clear(char **array, unsigned int size);

void foreach(one_device_results iterable);

int len(one_device_results results);

/*
 * method that makes unique object of game_result object
 */
inline one_device_results __init__(const string game_name, const string platform_name, unsigned int len) {
    const one_test_result test_result = {};
    const one_device_results to_return = {
        .game_name = game_name,
        .platform_name = platform_name,
        .stages_res = test_result
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

inline int len(one_device_results results) {
    int count = 0;
    for (auto __: results.stages_res) {
        count++;
    }
    return count;
}

/*
 * for each inner element print
 */
inline void foreach(const one_device_results iterable, unknown func) {
    for (auto __: iterable.stages_res) {
        func(__); //TODO может быть ошибка из -за того, что не ссылка
    }
}

inline void enter_data(one_test_result to_input, const bool is_pass, const string test_name, const string errors) {
   to_input.pass = is_pass;
    to_input.name = test_name;
    to_input.errors_in_test = errors;
}
