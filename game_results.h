game_results __init__(const string game_name, const string platform_name, unsigned int len);

void __clear__(char **array, unsigned int size);

int len(struct game_result results);

void foreach(game_result iterable);

typedef char Color;
typedef char string;

Color red = "\033[31m";
Color blue = "\033[34m";
Color green = "\033[32m";
Color yellow = "\033[33m";
Color cReset = "\033[0m";

/*
 * method that makes unique object of game_result object
 */
game_results __init__(const string game_name, const string platform_name, unsigned int len) {
    game_results to_return = {
            .game_name = game_name,
            .platform_name = platform_name,
            .test_res = one_test_result[len]};
    return to_return;
}

/*
 * method thar clears array
 */
void __clear__(char **array, unsigned int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

int len(struct game_result results) {
    int count = 0;
    for (auto __: results.test_res) {
        count++;
    }
    return count;
}

/*
 * for each inner element print
 */
void foreach(const game_result iterable) {
    for (auto __: iterable.test_res) {
        printf(__);
    }
}
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
    one_test_result test_res[];

    void *init = __init__();
    void *clear = __clear__();
    void *len = len();
    void *foreach = foreach();
} game_results;
