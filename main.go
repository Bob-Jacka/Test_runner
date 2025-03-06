/*
				Test runner.
Program for testing, written in GO language version.

author - cupcake_WRLD
version - 2.2.1

Test runner architecture:
                             Program entry point
									main
                                 Atos | Atos
                                 game_tests
									  |
                                 suit_proceed_______________       ,  print{Error, MSG}, recreate_slice..., colored_txt_output
                                     /  \                   \
                       str_user_input    get_results(Atob())  int_user_input
                                           /      \
                      print_results_to_console    write_results_to_file
*/

package main

import (
	"bufio"
	"fmt"
	"os"
	"runtime"
	"strconv"
	"strings"
	"time"
)

/*
All valuable const.
*/
const (
	red     Color = "\033[31m"
	blue    Color = "\033[34m"
	green   Color = "\033[32m"
	yellow  Color = "\033[33m"
	magenta Color = "\033[35m"
	cyan    Color = "\033[36m"
	gray    Color = "\033[37m"
	white   Color = "\033[97m"
	cReset  Color = "\033[0m"

	user_input_sign = ">> "
	success_test    = "success"
	failure_test    = "failure"
	skip_test       = "skip"
	help_test       = "help"

	ignore_line = "*" // Ignore line in test suit if it starts with this sign.
	nested_suit = "$" // Represents line is test suit as another test suit if it starts with this sign.

	fail_check_file   = "Error occurred in checking file."
	fail_check_dir    = "Error occurred in checking dir."
	fail_on_write     = "Error occurred in writing file."
	fail_on_open_file = "Error occurred during file action."

	str_input_err = "Error occurred in str_user_input."
	int_input_err = "Error occurred in int_user_input."

	file_name_ext = "results.txt"

	every_test_msg = "Enter (yes, 1) if success or (no, 0) for no if not or skip to skip."
	on_bug_msg     = "Write down what was wrong: "
	on_save_msg    = "Saving in file."
	wrong_arg      = "Wrong argument, try again."

	spend_time_on_test = "Времени на тестирование ушло: "
	hours              = "\tHours - "
	minutes            = "\tMinutes - "
	seconds            = "\tSeconds - "
	none_type          = "None"

	other_info    = "Other info:"
	go_comp       = "\tGo compiler is "
	comp_arch     = "\tComputer arch at time is "
	max_processes = "\tMax available processors are "
)

// Определенный типы для программы.
type (
	Color string

	/*
		Структура с этапами.
	*/
	test_stages_struct struct {
		stages []string
	}

	/*
		Структура с результатами каждой игры.
	*/
	test_results_struct struct {
		devices          []string
		every_device_res []string
	}
)

var (
	test_results [][]string // Первый массив - игра, второй массив - результаты тестов каждой игры.
	test_stages  []string   // Этапы тестирования.

	playsments_list []string // Список, в котором содержатся устройства для тестирования.

	start_time time.Time // Время начала тестирования.
	end_time   time.Time // Время конца тестирования.

	devices_count int16 = 0 // Количество устройств.
	tests_count   int   = 0 // Количество тестов в тестовом наборе.
)

/*
Точка входа в программу.
Принимает аргументы консоли и на их основании делает дальнейшее выполнение.
*/
func main() {
	var args_len = len(os.Args)
	switch args_len {
	case 1:
		colored_txt_output("Utility usage:", blue)
		colored_txt_output("First cli argument is <Test suit>", blue)
		colored_txt_output("*Optional arg - Second cli argument is <Devices List>", blue)
		colored_txt_output("*Optional arg - Third cli argument is <true / false write results to file>", blue)
		fmt.Println()
	case 2:
		game_tests(os.Args[1], none_type)
	case 3:
		game_tests(os.Args[1], os.Args[2])
		get_results(Atob("false"))
	case 4:
		game_tests(os.Args[1], os.Args[2])
		get_results(Atob(os.Args[3]))
	default:
		colored_txt_output(wrong_arg, red)
	}
	defer func() {
		fmt.Println()
		colored_txt_output("Bye!", magenta)
		os.Exit(666)
	}()
}

/*
Более общая функция для тестирования.
Комбинирует устройства с тестовыми наборами и проводит для каждого устройства тестовый набор.
*/
func game_tests(game_stages_cli string, games_devices_cli string) {
	start_time = time.Now()

	test_stages = Atos(game_stages_cli, false)
	tests_count = len(test_stages)
	playsments_list = Atos(games_devices_cli, true)
	if devices_count != 0 {
		test_results = recreate_double_slice(games_devices_cli)
	} else {
		colored_txt_output("An error occurred in making array test_results.", red)
		os.Exit(1)
	}

	for device_num := 0; device_num < len(playsments_list); device_num++ {
		colored_txt_output(strings.ToUpper(playsments_list[device_num]), blue)
		suit_proceed(device_num)
	}
	end_time = time.Now()
}

/*
Главная функция для тестирования.
Предоставляет тест из набора и запрашивает пользовательский ввод для результата.
Применяется для одного устройства.
*/
func suit_proceed(device_num int) {
	var stages_result = make([]string, tests_count)
	for stage_num := 0; stage_num < len(test_stages); stage_num++ {
		colored_txt_output(strings.ToUpper(test_stages[stage_num]), white)
		colored_txt_output("\t"+every_test_msg, blue)

		var user_input = str_user_input("", green)
		//if user_input_sign == help_test {
		//	help_menu()
		//}
		var bool_res = reverse_scan(user_input)
		if bool_res == true {
			stages_result[stage_num] = test_stages[stage_num] + " - No errors"
		} else if !bool_res && user_input == skip_test {
			stages_result[stage_num] = test_stages[stage_num] + " - TEST SKIPPED"
		} else if !bool_res {
			var problems = str_user_input(on_bug_msg, red)
			stages_result[stage_num] = test_stages[stage_num] + " - " + problems
		}
	}
	test_results[device_num] = append(stages_result)
	stages_result = recreate_slice2()
}

/*
Функция для распоряжения отображением результатов тестирования.
*/
func get_results(is_write_to_file bool) {
	if !is_write_to_file {
		print_results_to_console()
	} else {
		colored_txt_output(on_save_msg, green)
		write_results_to_file()
	}
}

/*
Вывод результатов тестирования в консоль.
*/
func print_results_to_console() {
	fmt.Println()
	for playsment_num := 0; playsment_num < len(test_results); playsment_num++ {
		if playsment_num < len(playsments_list) {
			colored_txt_output(strings.ToUpper(playsments_list[playsment_num]), blue)
			for stage := 0; stage < len(test_results[playsment_num]); stage++ {
				var res = test_results[playsment_num][stage]
				if res != "" {
					colored_txt_output("\t"+res, blue)
				}
			}
		}
	}
	var testing_duration = end_time.Sub(start_time)
	printMSG("", "")
	colored_txt_output(spend_time_on_test, yellow)
	colored_txt_output(hours+strconv.FormatFloat(testing_duration.Hours(), 'f', 0, 64), yellow)
	colored_txt_output(minutes+strconv.FormatInt(int64(testing_duration.Minutes()), 5), yellow)
	colored_txt_output(seconds+strconv.FormatFloat(testing_duration.Seconds(), 'f', 2, 64), yellow)
	printMSG("", "")
	colored_txt_output(other_info, yellow)
	colored_txt_output(go_comp+runtime.Compiler, yellow)
	colored_txt_output(comp_arch+runtime.GOARCH, yellow)
	colored_txt_output(max_processes+strconv.FormatInt(int64(runtime.GOMAXPROCS(runtime.NumCPU())), 10), yellow)
}

/*
Функция для записи результатов тестирования в файл.
*/
func write_results_to_file() {
	var file, err = os.Create(file_name_ext)
	var save_chan = make(chan string)
	if err != nil {
		printError(fail_on_write)
		os.Exit(1)
		return
	} else {
		print_results_chan(save_chan)
	}
	var writer = bufio.NewWriter(file)
	for str := range save_chan {
		_, err := writer.WriteString(str)
		if err != nil {
			printError(fail_on_write)
			return
		}
	}
	defer func(file *os.File) {
		err := file.Close()
		if err != nil {
			printError(fail_on_write)
		}
	}(file)
}

/*
Функция для записи результатов тестирования в "канал".
*/
func print_results_chan(chan_to_write chan string) {
	fmt.Println()
	for game_num := 0; game_num < len(test_results); game_num++ {
		fmt.Println(cReset)
		colored_txt_output(strings.ToUpper(playsments_list[game_num]), blue)
		chan_to_write <- strings.ToUpper(playsments_list[game_num])
		for stage := 0; stage < len(test_results[game_num]); stage++ {
			var res = test_results[game_num][stage]
			if res != "" {
				colored_txt_output("\t"+res, blue)
				chan_to_write <- "\t" + res
			}
		}
	}
	var duration = end_time.Sub(start_time)
	fmt.Println()
	colored_txt_output(spend_time_on_test, yellow)
	chan_to_write <- spend_time_on_test

	colored_txt_output(hours, yellow)
	chan_to_write <- hours

	colored_txt_output(duration.Hours(), yellow)
	chan_to_write <- strconv.FormatFloat(duration.Hours(), 'f', 2, 64)

	colored_txt_output(minutes, yellow)
	chan_to_write <- minutes

	colored_txt_output(duration.Minutes(), yellow)
	chan_to_write <- strconv.FormatInt(int64(duration.Minutes()), 10)

	colored_txt_output(seconds, yellow)
	chan_to_write <- seconds

	colored_txt_output(duration.Seconds(), yellow)
	chan_to_write <- strconv.FormatFloat(duration.Seconds(), 'f', 2, 64)

	fmt.Println()
	colored_txt_output(other_info, yellow)
	chan_to_write <- other_info

	colored_txt_output(go_comp+runtime.Compiler, yellow)
	chan_to_write <- go_comp + runtime.Compiler

	colored_txt_output(comp_arch+runtime.GOARCH, yellow)
	chan_to_write <- comp_arch + runtime.GOARCH

	colored_txt_output(max_processes, yellow)
	chan_to_write <- max_processes

	fmt.Println(runtime.GOMAXPROCS(runtime.NumCPU()))
	chan_to_write <- strconv.FormatInt(int64(runtime.GOMAXPROCS(runtime.NumCPU())), 10)
}

/*
Функция для трансформации введенной строки в булево значение.
*/
func reverse_scan(scan_val string) bool {
	switch scan_val {
	case "yes", "y", "ye", "1":
		return true
	case "no", "n", "0":
		return false
	case skip_test:
		return false
	case help_test:
		return false
	default:
		colored_txt_output(wrong_arg, red)
		var recurse_txt = str_user_input("", green)
		return reverse_scan(recurse_txt)
	}
}

/*
Меня с возможными действиями, по типу сохранения прогресса во время тестирования, загрузка прогресса и т.д.
*/
func help_menu() {
	colored_txt_output("Actions:", white)
	colored_txt_output("1. Save progress,", white)
	colored_txt_output("2. Load progress,", white)
	colored_txt_output("3. See results,", white)
	colored_txt_output("4. Close menu.", white)
	var user_input = int_user_input("")
	switch user_input {
	case 1:
		break
	case 2:
		break // TODO доделать
	case 3:
		get_results(true)
		break
	case 4:
		break
	}
}

/*
Проверка того, что переданный путь это файл.
*/
func check_file(path string) bool {
	var _, err = os.Stat(path)
	if err != nil {
		colored_txt_output(fail_check_file, red)
		check_dir(path)
		if !strings.Contains(path, ".") {
			colored_txt_output("Maybe path is not contains file extension.", red)
		}
		os.Exit(1)
		return false
	} else {
		colored_txt_output("File exists.", green)
		return true
	}
}

/*
Проверка того, что переданный путь это директория.
*/
func check_dir(path string) bool {
	var _, smth = os.ReadDir(path)
	if smth != nil {
		colored_txt_output(fail_check_dir, red)
		os.Exit(1)
		return false
	} else {
		colored_txt_output("Dir exists.", green)
		return true
	}
}

/*
Перевод строки в булево значение.
*/
func Atob(str string) bool {
	switch str {
	case "true", "True", "1":
		return true
	case "false", "False", "0":
		return false
	default:
		printError(wrong_arg)
		os.Exit(1)
		return false
	}
}

/*
Функция для перевода строки в массив.
*/
func Atos(str string, increm bool) []string {
	if str != none_type {
		if check_file(str) {
			colored_txt_output("Argument is a file.", green)
			return proceed_file(str, increm)
		} else if strings.Contains(str, ",") {
			colored_txt_output("Using as separator ','", blue)
			var split_line = strings.Split(str, ",")
			return split_line
		} else {
			colored_txt_output("Using as separator ' ' *whitespace.", blue)
			var split_line = strings.Split(str, " ")
			return split_line
		}
	} else {
		devices_count = 1 // Делаем количество устройств равным 1, если устройство не передано.
		return []string{"SingleDevice"}
	}
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно).
Возвращается содержимое файла.
*/
func proceed_file(path string, increm bool) []string {
	var file, err = os.Open(path)
	if err != nil {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_open_file, red)
		}
		os.Exit(1)
	}
	var read_array []string
	var reader = bufio.NewReader(file)
	var scanner = bufio.NewScanner(reader)

	for scanner.Scan() {
		line := scanner.Text() //строка
		if len(line) > 0 && !str_starts_with(line) {
			read_array = append(read_array, line)
			if increm {
				devices_count++
			}
		}
	}
	defer func(file *os.File) {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_open_file, red)
		}
	}(file)
	return read_array
}

/*
Функция для проверки наличия первым символом другого символа.
*/
func str_starts_with(source string) bool {
	return strings.Contains(source, ignore_line) && strings.Index(source, ignore_line) == 0
}

/*
Функция для вывода сообщения об ошибке.
*/
func printError(msg any) {
	fmt.Println(msg, red)
}

/*
Вывод сообщения независимо от цвета.
*/
func printMSG(str any, clr Color) {
	fmt.Print(cReset)
	fmt.Println(clr, str)
	fmt.Print(cReset)
}

/*
Функция для вывода сообщения заданными цветами.
Гарантировано выведет сообщение указанным цветом и не изменит его для следующего вывода текста.
*/
func colored_txt_output(str any, clr Color) {
	fmt.Print(cReset)
	switch clr {
	case red, yellow, green, blue, white, magenta, gray, cyan:
		fmt.Println(clr, str)
	}
	fmt.Print(cReset)
}

/*
Функция для пользовательского ввода.
Обрабатывает внутренние ошибки.
*/
func str_user_input(topic string, clr Color) string {
	var user_input string
	if topic != "" {
		fmt.Println(clr, topic)
	}
	fmt.Print(user_input_sign, green)
	_, err := fmt.Scanln(&user_input)
	if err != nil {
		printError(int_input_err)
		return ""
	}
	return user_input
}

/*
Функция для пользовательского ввода.
Обрабатывает внутренние ошибки.
*/
func int_user_input(topic string) int {
	var user_input int
	if topic != "" {
		_, err := fmt.Scanln(&user_input)
		if err != nil {
			printError(str_input_err)
			return 0
		}
		return user_input
	} else {
		fmt.Print(topic)
		_, err := fmt.Scanln(&user_input)
		if err != nil {
			printError(str_input_err)
			return 0
		}
		return user_input
	}
}

/*
Функция для создания массива большей длины.
*/
func recreate_slice1(slice []string) []string {
	return make([]string, len(slice)+1, cap(slice)+1*2)
}

/*
Функция для создания массива статически заданной длины.
*/
func recreate_slice2() []string {
	return make([]string, tests_count)
}

/*
Функция для создания двойного массива повышенной длины.
*/
func recreate_double_slice(slice string) [][]string {
	return make([][]string, len(slice)+2)
}
