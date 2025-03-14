/*
Файл, содержащий функции для использования в приложении.
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
Пользовательский тип для обозначения ссылки на файл в файловой системе.
*/
type File = *os.File

/*
Функция оболочка для привидения строки в upper case.
*/
func to_upper(s string) string {
	return strings.ToUpper(s)
}

/*
Функция оболочка для возвращения текущего времени.
*/
func get_now_time() time.Time {
	return time.Now()
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
		colored_txt_output(fail_on_write, red)
		os.Exit(1)
		return
	} else {
		print_results_to_file(file)
	}
	defer func(file File) {
		err2 := file.Close()
		if err2 != nil {
			colored_txt_output(fail_on_write, red)
		}
	}(file)
}

/*
Функция для записи результатов тестирования в "канал".
*/
func print_results_to_file(fd File) {
	fmt.Println()
	var device_counter uint32
	for device_counter = 0; device_counter < devices_count; device_counter++ {
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
func __write_string__(fd File, str string, is_newline bool) {
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
		colored_txt_output(wrong_arg, red)
		os.Exit(1)
		return false
	}
}

/*
Функция для перевода строки в массив.
*/
func Atos(str string, device_increment bool) StringContainer {
	if str != none_type {
		if check_file(str) {
			colored_txt_output("Argument is a file.", green)
			return proceed_file(str, device_increment)
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
		return StringContainer{"Single_Device_Testing"}
	}
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно).
Возвращается содержимое файла.
Device_increment - параметр, означающий необходимо ли увеличивать число устройств.
Path - путь до файла.
*/
func proceed_file(path string, device_increment bool) StringContainer {
	var file, err = os.Open(path)
	if err != nil {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_file_action, red)
		}
		os.Exit(1)
	}
	var main_suit StringContainer
	var reader = bufio.NewReader(file)
	var scanner = bufio.NewScanner(reader)

	for scanner.Scan() {
		line := scanner.Text()
		if len(line) > 0 {
			if str_starts_with_another_suit(line) {
				colored_txt_output("Nested suit detected", green)
				var cut_value = strings.Replace(line, nested_suit_sign, "", 1)
				var nested_suit = proceed_file(cut_value, false)
				if nested_suit != nil && len(nested_suit) > 0 {
					main_suit = append(main_suit, nested_suit...)
				} else {
					colored_txt_output(fail_on_file_action, red)
				}
			} else if !str_starts_with_ignore(line) {
				main_suit = append(main_suit, line)
				if device_increment {
					devices_count++
				}
			}
		}
	}
	defer func(file File) {
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
	defer func(file File) {
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
	return strings.Contains(source, ignore_line_sign) && strings.Index(source, ignore_line_sign) == 0
}

/*
Функция проверки того, что строка это вложенный набор тестов.
*/
func str_starts_with_another_suit(source string) bool {
	return strings.Contains(source, nested_suit_sign) && strings.Index(source, nested_suit_sign) == 0
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
		colored_txt_output("Outing the program", green)
		os.Exit(0)
	} else if err != nil {
		colored_txt_output(str_input_err, red)
	} else if user_input == help_test {
		help_menu()
		colored_txt_output("Testing continues, enter test result of the previous test input", gray)
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
			colored_txt_output(int_input_err, red)
			return 0
		}
		return user_input
	}
}

/*
Функция для создания массива большей длины.
*/
func recreate_slice1(slice StringContainer) StringContainer {
	return make(StringContainer, len(slice)+1, cap(slice)+1*2)
}

/*
Функция для создания массива статически заданной длины.
*/
func recreate_slice2() StringContainer {
	return make(StringContainer, tests_count)
}

/*
Функция для создания двойного массива повышенной длины.
*/
func recreate_double_slice(slice string) DoubleStringContainer {
	return make(DoubleStringContainer, len(slice)+2)
}
