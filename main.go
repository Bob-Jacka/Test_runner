/*
				Test runner.
Program for testing, written in GO language app_version.

author - cupcake_WRLD

Test runner architecture:
                             (Program entry point)
									main
                                 Atos | Atos
                                   /     \
                           object_tests   get_results -> write_results_to_file -> print_results_to_file -> __write_string__
									  |
                  ______________suit_proceed, print{Error}, recreate_slice..., colored_txt_output
                 /                /     \
     write_problems  str_user_input     int_user_input
                           |
                     help_menu_______
                    /     |          \
       load_progress   get_results   write_results_to_file
                        |
               print_results_to_console

"\ / | ->" - элементы означают передачу управления нижележащей функции.
*/

package main

import (
	"bufio"
	"fmt"
	"io"
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

	fail_check_file     = "Error occurred in checking file."
	fail_check_dir      = "Error occurred in checking dir."
	fail_on_write       = "Error occurred in writing file."
	fail_on_file_action = "Error occurred during file action."

	str_input_err      = "Error occurred in str_user_input."
	write_problems_err = "Error occurred in write_problems."
	int_input_err      = "Error occurred in int_user_input."

	test_res_error = "An error occurred in making array test_results."

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

	app_version = "2.4.2" // Версия приложения
)

// Определенные типы для программы.
type (

	/*
		    Пользовательский тип для цветов.
			Используется в цветном выводе в консоль
	*/
	Color string
)

/*
Основные глобальные переменные.
*/
var (
	test_results [][]string // Первый массив - объект тестирования, второй массив - результаты тестов каждого объекта.
	test_stages  []string   // Этапы тестирования.

	devices_list []string // Список, в котором содержатся устройства для тестирования.

	start_time time.Time // Время начала тестирования.
	end_time   time.Time // Время конца тестирования.

	devices_count int = 0 // Количество устройств на которых должно быть проведено тестирование.
	tests_count   int = 0 // Количество тестов в тестовом наборе.
)

/*
Точка входа в программу.
Принимает аргументы консоли и на их основании делает дальнейшее выполнение.
*/
func main() {
	var args_len = len(os.Args) - 1 // минус 1, потому что 1-й аргумент это имя файла.
	switch args_len {
	case 0:
		colored_txt_output("Utility usage:", blue)
		colored_txt_output("First cli argument is <Test suit>", blue)
		colored_txt_output("*Optional arg - Second cli argument is <Devices List>", blue)
		colored_txt_output("*Optional arg - Third cli argument is <true / false write results to file>", blue)
	case 1:
		object_tests(os.Args[1], none_type)
	case 2:
		object_tests(os.Args[1], os.Args[2])
		get_results(false)
	case 3:
		object_tests(os.Args[1], os.Args[2])
		get_results(Atob(os.Args[3]))
	default:
		colored_txt_output(wrong_arg, red)
	}
	defer func() {
		colored_txt_output("Bye!", magenta)
		os.Exit(666)
	}()
}

/*
Более общая функция для тестирования.
Комбинирует устройства с тестовыми наборами и проводит для каждого устройства тестовый набор.
*/
func object_tests(object_stages_cli string, object_devices_cli string) {
	start_time = time.Now()

	test_stages = Atos(object_stages_cli, false)
	tests_count = len(test_stages)
	devices_list = Atos(object_devices_cli, true)
	if devices_count != 0 {
		test_results = recreate_double_slice(object_devices_cli)
	} else {
		colored_txt_output(test_res_error, red)
		os.Exit(1)
	}

	for device_num := 0; device_num < len(devices_list); device_num++ {
		colored_txt_output(strings.ToUpper(devices_list[device_num]), blue)
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
		var bool_res = reverse_scan(user_input)
		if bool_res == true {
			stages_result[stage_num] = test_stages[stage_num] + " - No errors"
		} else if !bool_res && (user_input == skip_test || user_input == "") { // Был баг, если нажать перенос строки и затем валидный токен, то переносило в секцию багов.
			stages_result[stage_num] = test_stages[stage_num] + " - TEST SKIPPED"
		} else if !bool_res {
			var problems = write_problems(on_bug_msg, red)
			stages_result[stage_num] = test_stages[stage_num] + " - " + problems
		}
	}
	test_results[device_num] = append(stages_result)
	stages_result = recreate_slice2()
}

/*
Функция для распоряжения отображением результатов тестирования.
Возможные варианты - отображение в консоль или запись в файл.
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
		if playsment_num < len(devices_list) {
			colored_txt_output(strings.ToUpper(devices_list[playsment_num]), blue)
			for stage := 0; stage < len(test_results[playsment_num]); stage++ {
				var res = test_results[playsment_num][stage]
				if res != "" {
					colored_txt_output("\t"+res, blue)
				}
			}
		}
	}
	var testing_duration = end_time.Sub(start_time)
	fmt.Println()
	colored_txt_output(spend_time_on_test, yellow)
	colored_txt_output(hours+strconv.FormatFloat(testing_duration.Hours(), 'f', 0, 64), yellow)
	colored_txt_output(minutes+strconv.FormatInt(int64(testing_duration.Minutes()), 5), yellow)
	colored_txt_output(seconds+strconv.FormatFloat(testing_duration.Seconds(), 'f', 2, 64), yellow)
	fmt.Println()
	colored_txt_output(other_info, yellow)
	colored_txt_output(go_comp+runtime.Compiler, yellow)
	colored_txt_output(comp_arch+runtime.GOARCH, yellow)
	colored_txt_output(max_processes+strconv.FormatInt(int64(runtime.GOMAXPROCS(runtime.NumCPU())), 10), yellow)
}

/*
Функция для записи результатов тестирования в файл.
Запись результатов происходит через строковый канал.
*/
func write_results_to_file() {
	var file, err = os.Create(file_name_ext)
	if err != nil {
		printError(fail_on_write)
		os.Exit(1)
		return
	} else {
		print_results_to_file(file)
	}
	defer func(file *os.File) {
		err2 := file.Close()
		if err2 != nil {
			printError(fail_on_write)
		}
	}(file)
}

/*
Функция для записи результатов тестирования в "канал".
*/
func print_results_to_file(fd *os.File) {
	fmt.Println()
	for device_counter := 0; device_counter < devices_count; device_counter++ {
		fmt.Println(cReset)
		colored_txt_output(strings.ToUpper(devices_list[device_counter]), blue)
		__write_string__(fd, strings.ToUpper(devices_list[device_counter]), true)
		for stage := 0; stage < len(test_results[device_counter]); stage++ {
			var res = test_results[device_counter][stage]
			if res != "" {
				colored_txt_output("\t"+res, blue)
				__write_string__(fd, "\t"+res, true)
			}
		}
	}
	var duration = end_time.Sub(start_time)
	fmt.Println()
	__write_string__(fd, "\n", false)
	colored_txt_output(spend_time_on_test, yellow)
	__write_string__(fd, spend_time_on_test, true)

	colored_txt_output(hours, yellow)
	__write_string__(fd, hours, false)

	colored_txt_output(duration.Hours(), yellow)
	__write_string__(fd, strconv.FormatFloat(duration.Hours(), 'f', 2, 64), true)

	colored_txt_output(minutes, yellow)
	__write_string__(fd, minutes, false)

	colored_txt_output(duration.Minutes(), yellow)
	__write_string__(fd, strconv.FormatInt(int64(duration.Minutes()), 10), true)

	colored_txt_output(seconds, yellow)
	__write_string__(fd, seconds, false)

	colored_txt_output(duration.Seconds(), yellow)
	__write_string__(fd, strconv.FormatFloat(duration.Seconds(), 'f', 2, 64), true)

	fmt.Println()
	colored_txt_output(other_info, yellow)
	__write_string__(fd, other_info, true)

	colored_txt_output(go_comp+runtime.Compiler, yellow)
	__write_string__(fd, go_comp+runtime.Compiler, true)

	colored_txt_output(comp_arch+runtime.GOARCH, yellow)
	__write_string__(fd, comp_arch+runtime.GOARCH, true)

	colored_txt_output(max_processes, yellow)
	__write_string__(fd, max_processes, false)

	fmt.Println(runtime.GOMAXPROCS(runtime.NumCPU()))
	__write_string__(fd, strconv.FormatInt(int64(runtime.GOMAXPROCS(runtime.NumCPU())), 10), false)
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
	default:
		colored_txt_output(wrong_arg, red)
		var recurse_txt = str_user_input("", green)
		return reverse_scan(recurse_txt)
	}
}

/*
Приватная функция для ввода строки в файл.
Обрабатывает ошибки.
Fd - Файловый дескриптор.
Str - Строка для ввода в файл.
Is_newline - Необходимо ли переносить строку после ввода текста в файл.
*/
func __write_string__(fd *os.File, str string, is_newline bool) {
	_, err := fd.WriteString(str)
	if is_newline {
		_, err = fd.WriteString("\n")
	}
	if err != nil {
		colored_txt_output("Error occurred while low level writing to file: "+err.Error(), blue)
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
	colored_txt_output("4. App app_version,", white)
	colored_txt_output("5. Close menu.", white)
	fmt.Println("Enter action number:")
	fmt.Print(user_input_sign)
	var user_input = int_user_input("")
	switch user_input {
	case 1:
		write_results_to_file()
		break
	case 2:
		colored_txt_output("Loading last test result", blue)
		var loaded_string = load_progress()
		colored_txt_output(loaded_string, green)
		break
	case 3:
		get_results(false)
		break
	case 4:
		colored_txt_output("App version is "+app_version, white)
		break
	case 5:
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
		return []string{"Single_Device_Testing"}
	}
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно).
Возвращается содержимое файла.
Increm - параметр, означающий необходимо ли увеличивать число устройств.
Path - путь до файла.
*/
func proceed_file(path string, increm bool) []string {
	var file, err = os.Open(path)
	if err != nil {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_file_action, red)
		}
		os.Exit(1)
	}
	var main_suit []string
	var reader = bufio.NewReader(file)
	var scanner = bufio.NewScanner(reader)

	for scanner.Scan() {
		line := scanner.Text()
		if len(line) > 0 {
			if str_starts_with_another_suit(line) {
				colored_txt_output("Nested suit detected", green)
				var nested_suit = proceed_file(strings.Replace(line, nested_suit, "", 1), false)
				if nested_suit != nil && len(nested_suit) > 0 {
					main_suit = append(main_suit, nested_suit...)
				} else {
					colored_txt_output(fail_on_file_action, red)
				}
			} else if !str_starts_with_ignore(line) {
				main_suit = append(main_suit, line)
				if increm {
					devices_count++
				}
			}
		}
	}
	defer func(file *os.File) {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_file_action, red)
		}
	}(file)
	return main_suit
}

/*
Функция для загрузки информации о прохождении тестирования.
Возвращает последний тест из сохраненного результата.
*/
func load_progress() string {
	var file, err = os.Open(file_name_ext)
	if err != nil {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_file_action, red)
		}
		os.Exit(1)
	}
	var reader = bufio.NewReader(file)
	var scanner = bufio.NewScanner(reader)
	var last_test string
	for scanner.Scan() {
		line := scanner.Text()
		if line != io.EOF.Error() {
			last_test = line
		}
	}
	defer func(file *os.File) {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_file_action, red)
		}
	}(file)
	return last_test
}

/*
Функция для проверки наличия первым символом - символа игнорирования строки.
Возвращает true - если строка начинается с символа игнорирования, false - если не начинается.
*/
func str_starts_with_ignore(source string) bool {
	return strings.Contains(source, ignore_line) && strings.Index(source, ignore_line) == 0
}

/*
Функция проверки того, что строка это вложенный набор тестов.
*/
func str_starts_with_another_suit(source string) bool {
	return strings.Contains(source, nested_suit) && strings.Index(source, nested_suit) == 0
}

/*
Функция для вывода сообщения об ошибке.
*/
func printError(msg any) {
	fmt.Println(msg, red)
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
	fmt.Print(user_input_sign, cyan)
	_, err := fmt.Scanln(&user_input)
	if err != nil && err.Error() != "unexpected newline" {
		colored_txt_output(str_input_err, red)
		return ""
	} else if user_input == help_test {
		help_menu()
		colored_txt_output("Testing continues", gray)
		user_input = str_user_input(topic, clr)
	}
	return user_input
}

/*
Функция для строкового ввода с пробелами.
Возвращает введенную строку.
*/
func write_problems(topic string, clr Color) string {
	var reader = bufio.NewReader(os.Stdin)
	if topic != "" {
		fmt.Println(clr, topic)
	}
	fmt.Print(user_input_sign, cyan)
	var read, err = reader.ReadString('\n')
	if err != nil {
		colored_txt_output(write_problems_err, red)
		return ""
	}
	return strings.TrimSpace(read)
}

/*
Функция для пользовательского ввода.
Обрабатывает внутренние ошибки.
*/
func int_user_input(topic string) int {
	var user_input int
	if topic != "" {
		_, err := fmt.Scanln(&user_input, cyan)
		if err != nil && err.Error() != "unexpected newline" {
			colored_txt_output(int_input_err, red)
			return 0
		}
		return user_input
	} else {
		fmt.Print(topic)
		_, err := fmt.Scanln(&user_input)
		if err != nil {
			printError(int_input_err)
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
