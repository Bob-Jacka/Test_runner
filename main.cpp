#include "List.h"
#include "static_funcs.h"
#include "Game_results.h"
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
                time_t timestamp;
                println_info("Saving current testing progress");
                println("Enter stage");
                const_string save_point_stages = input();
                println("Enter device");
                const_string save_point_device = input();
                ctime(&timestamp);
                string save_path = strcat(input_statements.save_file_name + timestamp, ".txt");
                std::ofstream out(save_path, std::ios::app);
                if (out.is_open()) {
                    out << save_point_stages << std::endl;
                    out << save_point_device << std::endl;
                } else {
                    println_error("Failed to open save file");
                }
                out.close();
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
