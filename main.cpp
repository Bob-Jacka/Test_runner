#include <fstream>
#include <cstdlib>
#include "List.h"
#include "static_funcs.h"
#include "prototypes.h"
#include "game_results.h"

List<List<one_device_results> > all_stages_test_results;
//|First array| - game, |Second array| - each game test results

List<string> test_stages;
List<string> games_list;

time_t start_time;
time_t end_time;

counter_v devices_count;
counter_v stage_count;

int main(int argc, char *argv[]) {
    const List<string> args = split_string(get_string_by_chars(*argv), *" ");
    //TODO add game name to arguments
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
            printMSG("Given arguments - ");
            println_error("Error in arguments");
            gracefully_exit();
        }
    }
    println("Bye");
}

void game_tests(const_string &stages_cli, const string &devices_cli) {
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
        printMSG(">> ");
        const string res = reverse_scan();
        string problems;
        stages_result.setElement(stage, {});
        if (res == SUCCESS) {
            enter_data(stages_result.getElement(stage), true, test_stages.getElement(stage).append(SUCCESS),
                       problems);
        } else if (res == SKIPPED) {
            enter_data(stages_result.getElement(stage), true, test_stages.getElement(stage).append(SKIPPED),
                       problems);
        } else if (res == FAILURE) {
            printMSG("Write down, what was wrong: ");
            const counter_v _ = scanf("%s", &problems); //TODO учитывает только одно слово без пробелов
            if (_ > 0) {
                println_error("an error occurred in section about something bad in test");
                return;
            }
            enter_data(stages_result.getElement(stage), false, test_stages.getElement(stage).append(FAILURE),
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
        int option = 0;
        printMSG(">> ");
        scanf("%d", &option);
        if (isdigit(option) == true) {
            switch (option) {
                case 1: {
                    println_info("Saving current testing progress");
                    break;
                }
                case 2: {
                    println_info("Current results are:");
                    // counter_v device_num = 0;
                    // for (one_device_results _1: all_stages_test_results.getElement(device_num)) {
                    //     one_device_results outer_array_game = _1;
                    //     counter_v inner_array_test = 0; //TODO maybe error because double array
                    //     for (one_test_result _2: outer_array_game) {
                    //         println(_2.get_name().c_str());
                    //         println(_2.get_pass());
                    //         println(_2.get_errors_in_test().c_str());
                    //     }
                    //     device_num++;
                    // }
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
        } else {
            println_error("Invalid argument type");
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
    string user_input;
    const unsigned int _ = scanf("%s", &user_input);
    if (_ > 0) {
        println_error("an error occurred in scan game stage result");
    }
    if (user_input == "yes" || user_input == "1") {
        return SUCCESS;
    }
    if (user_input == "no" || user_input == "0") {
        return FAILURE;
    }
    if (user_input == "skip") {
        return SKIPPED;
    }
    if (user_input == EXIT) {
        gracefully_exit();
    }
    if (user_input == HELP_short || user_input == HELP_long) {
        get_help_menu();
    }

    println_error("Please, try again");
    printMSG(">> ");
    return reverse_scan();
}

/*
Проверка того, что переданный путь это файл
*/
bool check_file(const_string &path) {
    std::ifstream in;
    in.open(path);
    if (in.is_open()) {
        in.close();
        println_info("File exists");
        return true;
    }
    println_error("Error occurred in checking file");
    check_dir(path);
    if (path.find('.') == -1) {
        in.close();
        println_error("Maybe path is not contains file extension");
    }
    gracefully_exit();
    return false;
}

/*
Проверка того, что переданный путь это директория
*/
bool check_dir(const_string &path) {
    std::ifstream in;
    in.open(path);
    if (in.is_open()) {
        println_info("Dir exists");
        in.close();
        return true;
    }
    println_error("Error occurred is checking dir");
    in.close();
    gracefully_exit();
    return false;
}

/*
Перевод строки в массив
*/
List<string> Atos(const_string &str) {
    if (check_file(str)) {
        println_info("Argument is a file");
        return proceed_file(str);
    }
    if (str.find(',') == -1) {
        println_info("using as separator ','");
        List<string> strings = split_string(str, *",");
        return strings;
    }
    println_info("using as separator ' ' *whitespace");
    List<string> strings = split_string(str, *" ");
    return strings;
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно)
Возвращается содержимое файла
*/
List<string> proceed_file(const_string &path) {
    std::ifstream in;
    in.open(path);
    if (!in.is_open()) {
        println_error("error occurred during file open");
        gracefully_exit();
        return List<string>{};
    }
    printMSG("Dir exists");
    std::string fileLine;
    List<string> fileLines;
    while (std::getline(in, fileLine)) {
        if (fileLine != "\n") {
            if (!fileLine.starts_with(ignore_test)) {
                fileLines.addElement(fileLine);
            }
            if (fileLine.starts_with(another_suit)) {
                println_info("Another suit detected");
                // fileLines.addElement(proceed_file(fileLine));
            }
        }
    }
    in.close();
    return fileLines;
}
