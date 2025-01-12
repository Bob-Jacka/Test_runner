#include "List.h"
#include "Game_results.h"
#include "Inline_funcs.h"
#include "Input_statements.h"

void stages(counter_v device_num);
void tests(const_string &stages_cli, const string &devices_cli);
string reverse_scan();
void get_results(int is_write_to_file);
void print_results();
[[noreturn]] void get_help_menu();

//|First array| - game, |Second array| - each game test results
List<List<one_device_results> > all_stages_test_results;

List<string> test_stages;
List<string> devices_list;

time_t start_time;
time_t end_time;

counter_v stage_count;

int main(const int argc, char *argv[]) {
    const List<string> params = split_string(get_string_by_chars(*argv), *" ");
    switch (argc) {
        case 1: {
            println_important("Utility usage:");
            println_info("First cli argument is <Test stages>");
            println_info("Second cli argument is <Devices>");
            println_info("Third cli argument is <true / false write results to file> (optional)");
            break;
        }
        case 3: {
            tests(params.getElement(1), params.getElement(2));
            get_results(false);
            break;
        }
        case 4: {
            tests(params.getElement(1), params.getElement(2));
            get_results(std::stoi(params.getElement(3)));
            break;
        }
        default: {
            println_error("Error in arguments");
            gracefully_exit();
        }
    }
    println("Bye");
}

void tests(const_string &stages_cli, const_string &devices_cli) {
    ctime(&start_time);
    init_string_arr(test_stages, Atos(stages_cli));
    init_string_arr(devices_list, Atos(devices_cli));
    if (stage_count != 0) {
        init_array_by(all_stages_test_results, get_empty());
    } else {
        println_error("An error occurred in making array test_results");
        gracefully_exit();
    }
    for (counter_v device_num = 0; device_num < devices_list.getSize(); device_num++) {
        stages(device_num);
    }
    ctime(&end_time);
}

void stages(const counter_v device_num) {
    List<one_test_result> stages_result;
    println();
    println_important(devices_list.getElement(device_num));
    for (counter_v stage = 0; stage < test_stages.getSize(); stage++) {
        to_upper(test_stages.getElement(stage));
        const_string res = reverse_scan();
        string problems;
        stages_result.setElement(stage, {});
        if (res == TEST_SUCCESS) {
            enter_data(stages_result.getElement(stage), true,
                       test_stages.getElement(stage).append(TEST_SUCCESS),
                       problems);
        } else if (res == TEST_SKIPPED) {
            enter_data(stages_result.getElement(stage), true,
                       test_stages.getElement(stage).append(TEST_SKIPPED),
                       problems);
        } else if (res == TEST_FAILURE) {
            printMSG("Write down what was wrong: ");
            problems = input();
            enter_data(stages_result.getElement(stage), false,
                       test_stages.getElement(stage).append(TEST_FAILURE),
                       problems);
        }
        all_stages_test_results.getElement(device_num).getElement(stage).set_stages_res(
            stages_result.getElement(stage), stage);
    }
}

[[noreturn]] void get_help_menu() {
    println();
    println_important("This utility provide ability to run testing suites");
    println("Program points:");
    println("\t 1 - Save testing progress");
    println("\t 2 - Print current results");
    println("\t 3 - Get input cli parameters");
    println("\t 4 - Print test suit");
    println("\t 5 - Close menu");
    while (true) {
        counter_v option = input_int();
        switch (option) {
            case 1: {
                save_results(false);
                break;
            }
            case 2: {
                println_important("Current results are:");
                for (const auto &outer: all_stages_test_results) {
                    for (auto inner: outer) {
                        for (int i = 0; i < test_stages.getSize(); i++) {
                            auto test = inner.get_stages_res().getElement(i);
                            println("Test name: " + test.get_name());
                            println("Test status: " + test.get_pass());
                            println("Test errors / notes: " + test.get_errors_in_test());
                        }
                    }
                }
            }
            break;
            case 3: {
                println_important("Utility parameters");
                println_info("First param are 'Game stages' (ex. tests that you want to test)");
                println_info("Second param are 'Devices' (ex. android, ios, desktop)");
                println_info(
                    "Third param is optional, but it point to write tests result to file or not (true \\ false)");
                println();
                println_info("Test suite fill rules:");
                println_info("1. One line - one test");
                println_info("2. if you want to ignore some tests - enter '*' at test start");
                println_info(
                    "3. if you want to add another test suit in this test suit you need to add '$' at string start line");
                println_info("4. Do not include test suit in test suit in test suit");
                continue;
            }
            case 4: {
                println_important("Test suit:");
                for (const auto &inner: test_stages) {
                    println(inner);
                }
                break;
            }
            case 5: {
                break;
            }
            default:
                break;
        }
    }
}

void get_results(const int is_write_to_file) {
    if (!is_write_to_file) {
        print_results();
    } else {
        printMSG("Saving in file");
        save_results(true);
    }
}

void print_results() {
    println();
    for (counter_v game_num = 0; game_num < double_size(all_stages_test_results); game_num++) {
        println();
        to_upper(devices_list.getElement(game_num));
        for (counter_v stage = 0; stage < all_stages_test_results.getElement(game_num).getSize(); stage++) {
            const std::string res = all_stages_test_results.getElement(game_num).getElement(stage).get_stages_res().
                    getElement(stage).get_name();
            println_info(strcat("\t ", res.c_str()));
        }
    }
    const unsigned long dur = start_time - end_time;
    get_time_stat(dur);
}

string reverse_scan() {
    printMSG(EVERY_TEST_MESSAGE);
    const_string user_input = input();
    if (user_input == SKIP_CHOICE) {
        return TEST_SKIPPED;
    }
    if (user_input == FIRST_PASS_CHOICE || user_input == SECOND_PASS_CHOICE) {
        return TEST_SUCCESS;
    }
    if (user_input == FIRST_FAILURE_CHOICE || user_input == SECOND_FAILURE_CHOICE) {
        return TEST_FAILURE;
    }
    if (user_input == EXIT) {
        gracefully_exit();
    }
    if (user_input == SHORT_FLAG_CHOICE || user_input == LONG_FLAG_CHOICE) {
        get_help_menu();
    }
    println_error("Please, try again");
    return reverse_scan();
}
