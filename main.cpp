#include "List.h"
#include "static_funcs.h"
#include "game_results.h"
#include "input_statements.h"

void game_stages(unsigned int device_num);

void game_tests(const_string &stages_cli, const string &devices_cli);

string reverse_scan();

void get_results(int is_write_to_file);

void print_results();

[[noreturn]] void get_help_menu();

List<List<one_device_results> > all_stages_test_results;
//|First array| - game, |Second array| - each game test results

List<string> test_stages;
List<string> games_list;

time_t start_time;
time_t end_time;

counter_v devices_count;
counter_v stage_count;

int main(const int argc, char *argv[]) {
    const List<string> args = split_string(get_string_by_chars(*argv), *" ");
    switch (argc) {
        case 1: {
            println_info("Utility usage:");
            println_info("First cli argument is <Test stages>");
            println_info("Second cli argument is <Devices>");
            println_info("Third cli argument is <true / false write results to file>");
            println();
            return 0;
        }
        case 3: {
            constexpr bool hardcoded_bool_value = false;
            game_tests(args.getElement(1), args.getElement(2));
            get_results(hardcoded_bool_value);
            break;
        }
        case 4: {
            game_tests(args.getElement(1), args.getElement(2));
            get_results(std::stoi(args.getElement(3)));
            break;
        }
        default: {
            println_error("Error in arguments");
            gracefully_exit();
        }
    }
    println("Bye");
}

void game_tests(const_string &stages_cli, const_string &devices_cli) {
    ctime(&start_time);
    init_string_arr(test_stages, Atos(stages_cli));
    init_string_arr(games_list, Atos(devices_cli));
    if (stage_count != 0) {
        init_array_by(all_stages_test_results, get_empty());
    } else {
        println_error("An error occurred in making array test_results");
        gracefully_exit();
    }
    for (counter_v device_num = 0; device_num < games_list.getSize(); device_num++) {
        game_stages(device_num);
    }
    ctime(&end_time);
}

void game_stages(const counter_v device_num) {
    List<one_test_result> stages_result;
    println();
    println_important(games_list.getElement(device_num));
    for (counter_v stage = 0; stage < test_stages.getSize(); stage++) {
        to_upper(test_stages.getElement(stage));
        printMSG("Enter (yes / 1) for success or (no / 0) for failure or skip to skip");
        const_string res = reverse_scan();
        string problems;
        stages_result.setElement(stage, {});
        if (res == input_statements.SUCCESS) {
            enter_data(stages_result.getElement(stage), true,
                       test_stages.getElement(stage).append(input_statements.SUCCESS),
                       problems);
        } else if (res == input_statements.SKIPPED) {
            enter_data(stages_result.getElement(stage), true,
                       test_stages.getElement(stage).append(input_statements.SKIPPED),
                       problems);
        } else if (res == input_statements.FAILURE) {
            printMSG("Write down what was wrong: ");
            problems = input();
            enter_data(stages_result.getElement(stage), false,
                       test_stages.getElement(stage).append(input_statements.FAILURE),
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
                println_info("Saving current testing progress");
                break;
            }
            case 2: {
                println_info("Current results are:");
                counter_v device_num = 0;
                for (const one_device_results& _1: all_stages_test_results.getElement(device_num)) {
                    one_device_results outer_array_game = _1;
                    counter_v inner_array_test = 0; //TODO maybe error because double array
                    for (one_test_result _2: outer_array_game) {
                        println(_2.get_name().c_str());
                        println(_2.get_pass());
                        println(_2.get_errors_in_test().c_str());
                    }
                    device_num++;
                }
            }
            break;
            case 3: {
                println_info("Utility parameters");
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
                println_info("4. Enter 'problem' word if you want to enter something even if test is passed");
                continue;
            }
            case 4: {
                for (int i = 0; i < test_stages.getSize(); i++) {
                    printMSG(test_stages.getElement(i));
                }
                break;
            }
            case 5: {
                println("Bye");
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
        // write_to_file();
    }
}

void print_results() {
    println();
    for (counter_v game_num = 0; game_num < double_size(all_stages_test_results); game_num++) {
        println();
        to_upper(games_list.getElement(game_num));
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
    const_string user_input = input();
    if (user_input == input_statements.yes || user_input == input_statements.one) {
        return input_statements.SUCCESS;
    }
    if (user_input == input_statements.no || user_input == input_statements.zero) {
        return input_statements.FAILURE;
    }
    if (user_input == input_statements.skip) {
        return input_statements.SKIPPED;
    }
    if (user_input == input_statements.EXIT) {
        gracefully_exit();
    }
    if (user_input == input_statements.short_help || user_input == input_statements.long_help) {
        get_help_menu();
    }
    println_error("Please, try again");
    return reverse_scan();
}
