/*
Файл, содержащий функции для использования в приложении.
Функции используются в main.go файле.
*/

package main

import (
	ta "Test_runner/Test_artifacts"
	"bufio"
	"errors"
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
S - строка для возврата в upper case.
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
Is_write_to_file - булево значение записи в файл.
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
	for idx, device := range devices_list {
		colored_txt_output(strings.ToUpper(devices_list[idx]), blue)
		for stage := 0; stage < len(test_results[device]); stage++ {
			var res = test_results[device][stage]
			var msg = res.GetMessage()
			if msg != "" {
				colored_txt_output("\t"+msg, blue)
			}
		}
	}

	if len(bug_container) > 0 {
		colored_txt_output("\n", magenta)
		colored_txt_output("Найденные баги:", magenta)
		for _, elem := range bug_container {
			colored_txt_output(elem.To_string(), magenta)
		}
	}

	colored_txt_output(load_parameters.to_string(), yellow)
	fmt.Println()
	colored_txt_output(spend_time_on_test, yellow)
	var all_time = end_time.Sub(start_time).Seconds()
	var minute_time = all_time / 60.0
	var hour_time = all_time / 3600.0

	colored_txt_output(hours+strconv.FormatFloat(hour_time, 'f', 0, 64), yellow)
	colored_txt_output(minutes+strconv.FormatInt(int64(minute_time), 5), yellow)

	fmt.Println()
	colored_txt_output(other_info, yellow)
	colored_txt_output(go_comp+runtime.Compiler, yellow)
	colored_txt_output(comp_arch+runtime.GOARCH, yellow)
	colored_txt_output(max_processes+strconv.FormatInt(int64(runtime.NumCPU()), 10), yellow)
}

/*
Функция для записи результатов тестирования в файл.
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
Функция для записи результатов тестирования в файл.
Fd - файловый дескриптор для записи в файл.
*/
func print_results_to_file(fd File) {
	fmt.Println()
	for idx, device := range devices_list {
		fmt.Println(cReset)
		colored_txt_output(strings.ToUpper(devices_list[idx]), blue)
		__write_string__(fd, strings.ToUpper(devices_list[idx]), true)
		if test_results[device] != nil {
			for stage := 0; stage < len(test_results[device]); stage++ {
				var res = test_results[device][stage]
				var msg = res.GetMessage()
				if msg != "" {
					colored_txt_output("\t"+msg, blue)
					__write_string__(fd, "\t"+msg, true)
				}
			}
		} else {
			colored_txt_output("\tНабор тестов еще не закончен, вывод невозможен.", blue)
			__write_string__(fd, "\tНабор тестов еще не закончен, вывод невозможен.", true)
		}
	}
	fmt.Println()
	colored_txt_output(load_parameters.to_string(), yellow)
	__write_string__(fd, load_parameters.to_string(), true)

	var duration = end_time.Sub(start_time)
	fmt.Println()

	if len(bug_container) > 0 {
		__write_string__(fd, "", true)
		__write_string__(fd, "Найденные баги:", true)
		for _, elem := range bug_container {
			__write_string__(fd, elem.To_string(), true)
		}
	}

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

	fmt.Println(runtime.NumCPU())
	__write_string__(fd, strconv.FormatInt(int64(runtime.NumCPU()), 10), false)
}

/*
Функция для трансформации введенной строки в булево значение.
Scan_val - введенное пользователем значение для анализа.
*/
func reverse_scan(scan_val string) bool {
	switch scan_val {
	case "yes", "y", "ye", "1":
		return true
	case "no", "n", "0":
		return false
	case skip_test:
		return false
	case "\n":
		return false
	case out_prog:
		return false
	default:
		colored_txt_output(wrong_arg, red)
		var recurse_txt = str_user_input("", green, &ta.TestCase{}, ta.Undefined_field)
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
		colored_txt_output("Error occurred while low level writing to file: "+err.Error(), red)
	}
}

/*
Меня с возможными действиями, по типу сохранения прогресса во время тестирования, загрузка прогресса и т.д.
Save_point - название теста для сохранения.
Save_point_device - название устройства для сохранения.
*/
func help_menu(current_tc *ta.TestCase, save_point_device string) {
	colored_txt_output("Действия:", white)
	colored_txt_output("1. Сохранить прогресс,", white)
	colored_txt_output("2. Информация о текущем тест кейсе,", white)
	colored_txt_output("3. Загрузить прогресс,", white)
	colored_txt_output("4. Включить таймер,", white)
	colored_txt_output("5. Посмотреть результаты,", white)
	colored_txt_output("6. Посмотреть баги,", white)
	colored_txt_output("7. Версия приложения,", white)
	colored_txt_output("8. Закрыть меню.", white)
	fmt.Println("Введите номер действия:")
	fmt.Print(user_input_sign)
	var user_input, err = int_user_input("")
	if err == nil {
		switch user_input {
		case 1:
			end_time = get_now_time()
			var file, err = os.Create(save_file_name)
			if err != nil {
				colored_txt_output("Error occurred while creating save point: "+err.Error(), red)
			} else {
				__write_string__(file, "Остановился здесь - "+current_tc.Name+", Устройство - "+save_point_device, true)
				__write_string__(file, load_parameters.to_string(), true)
				if len(bug_container) > 0 {
					__write_string__(file, "", true)
					__write_string__(file, "Найденные баги:", true)
					for _, elem := range bug_container {
						__write_string__(file, elem.To_string(), true)
					}
				}
				err := file.Close()
				if err != nil {
					return
				}
			}
			break
		case 2:
			colored_txt_output("Информация о текущем тест кейсе:", blue)
			colored_txt_output("Название тест кейса - "+current_tc.Name, blue)
			colored_txt_output("Приоритет тест кейса - "+current_tc.Priority, blue)
			colored_txt_output("Вложенное сообщение - "+current_tc.Msg, blue)
		case 3:
			colored_txt_output("Загрузка результатов тестирования", blue)
			var loaded_string = load_progress()
			colored_txt_output(loaded_string, green)
			break
		case 4:
			var timer_count, _ = int_user_input("Введите количества минут для ожидания.")
			colored_txt_output("Таймер включен:", blue)
			colored_txt_output("Раз в минуту будет показываться время:", blue)
			for timer_count != 0 {
				colored_txt_output(timer_count, green)
				time.Sleep(60 * time.Second)
				timer_count--
			}
			break
		case 5:
			if devices_count == 1 {
				colored_txt_output("Получение результатов тестирования не доступно для режима одного устройства.", blue)
				break
			} else {
				get_results(false)
			}
			break
		case 6:
			for _, bug := range bug_container {
				colored_txt_output("\tИмя бага: "+bug.Name, white)
				colored_txt_output("\tПриоритет бага: "+bug.Priority, white)
				colored_txt_output("\tВажность бага: "+bug.Severity, white)
				colored_txt_output("\tВложенное сообщение: "+bug.Msg, white)
				fmt.Println()
			}
		case 7:
			colored_txt_output("Версия приложения - "+app_version, white)
			break
		case 8:
			break
		}
	} else {
		colored_txt_output("Error occurred while help menu input: ", red)
	}
}

/*
Проверка того, что переданный путь это файл.
Path - путь для проверки на файл.
Возвращает - булево значение, true - файл существует, false - не существует.
*/
func check_file(path string) bool {
	var _, err = os.Stat(path)
	if err != nil {
		colored_txt_output(fail_check_file, red)
		if !strings.Contains(path, ".") {
			colored_txt_output("Maybe, 'path' does not contain file extension.", red)
		}
		os.Exit(1)
		return false
	} else {
		colored_txt_output("Файл существует.", green)
		return true
	}
}

/*
Функция для перевода строки в массив.
Str - строка для распознавания.
*/
func Atos(str string) TestCaseContainer {
	if check_file(str) {
		colored_txt_output("Argument is a file.", green)
		return proceed_file_test_cases(str)
	} else {
		colored_txt_output("Return empty TestCaseContainer.", red)
		return nil
	}
}

/*
Функция для чтения устройств из файла.
Path - путь до файла с устройствами для тестирования.
*/
func get_devices(path string) *StringContainer {
	var file, err = os.Open(path)
	var main_suit StringContainer
	if err != nil && path == none_type {
		devices_count = 1
		return &StringContainer{"Single_device_mode"}
	} else if err != nil && path != none_type {
		var reader = bufio.NewReader(file)
		var scanner = bufio.NewScanner(reader)
		for scanner.Scan() {
			line := scanner.Text()
			if len(line) > 0 && !str_starts_with_ignore(line) {
				main_suit = append(main_suit, line)
			}
		}
	}
	defer func(file File) {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_file_action, red)
		}
	}(file)
	devices_count = uint32(len(main_suit))
	return &main_suit
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно).
Возвращается содержимое файла, даже если файл содержит вложенные наборы.
Path - путь до файла.
*/
func proceed_file_test_cases(path string) TestCaseContainer {
	var file, err = os.Open(path)
	if err != nil {
		err := file.Close()
		if err != nil {
			colored_txt_output(fail_on_file_action, red)
		}
		os.Exit(1)
	}
	var main_suit TestCaseContainer
	var reader = bufio.NewReader(file)
	var scanner = bufio.NewScanner(reader)

	for scanner.Scan() {
		line := scanner.Text()
		if len(line) > 0 {
			if str_starts_with_another_suit(line) {
				colored_txt_output("Nested suit detected", green)
				var nested_suit_value = strings.Replace(line, nested_suit_sign, "", 1)
				var nested_suit = proceed_file_test_cases(nested_suit_value)
				if nested_suit != nil && len(nested_suit) > 0 {
					main_suit = append(main_suit, nested_suit...) // Добавление вложенного набора тестов.
				} else {
					colored_txt_output(fail_on_file_action, red)
				}
			} else if !str_starts_with_ignore(line) {
				if !str_ends_with_multiline(line) {
					main_suit = add_tc_to_test_case(main_suit, line)
				} else {
					main_suit = add_tc_to_test_case(main_suit, strings.ReplaceAll(line, "/", ""))
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
Создает объект тест кейса из приведенной строки и добавляет в контейнер.
Container - контейнер для тест кейсов.
Line - строка из тестового набора для преобразования в тест кейс.
*/
func add_tc_to_test_case(container TestCaseContainer, line string) TestCaseContainer {
	var tc, _ = proceed_string_to_test_case(line)
	var to_add, err = ta.Construct_test_case(tc)
	if err != nil {
		colored_txt_output("Error occurred during test case construct - "+err.Error(), red)
	}
	return append(container, to_add)
}

/*
Функция для разбиения строки на карту значений.
Line - строка текста для преобразования в тест кейс.
Возвращает - карту с ключом строка и значением строка.
*/
func proceed_string_to_test_case(line string) (map[string]string, error) {
	var line_split = strings.Split(line, test_case_split_sign)
	if len(line_split) > 1 && strings.Contains(line, test_case_split_sign) {
		var to_return = make(map[string]string, 4)
		to_return[ta.Name_field] = line_split[0]
		to_return[ta.Priority_field] = line_split[1]
		to_return[ta.Msg_field] = line_split[2]
		return to_return, nil
	} else {
		colored_txt_output("Error occurred during proceed string to test case.", red)
		return nil, errors.New("line length cannot be less than 1")
	}
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
Source - строка для проверки.
*/
func str_starts_with_ignore(source string) bool {
	return strings.Contains(source, ignore_line_sign) && strings.Index(source, ignore_line_sign) == 0
}

/*
Функция проверки того, что строка это вложенный набор тестов.
Source - строка для проверки.
*/
func str_starts_with_another_suit(source string) bool {
	return strings.Contains(source, nested_suit_sign) && strings.Index(source, nested_suit_sign) == 0
}

/*
Функция проверки того, что строка мультистрочное название.
Source - строка для проверки.
*/
func str_ends_with_multiline(source string) bool {
	return strings.Contains(source, multiline_test_sign) && strings.LastIndex(source, multiline_test_sign) != 0
}

/*
Функция для пользовательского ввода.
Обрабатывает внутренние ошибки.
Topic - опциональная строка для отображения до пользовательского ввода.
Current_tc - объект текущего тест кейса.
Save_point_device - название устройства для сохранения.
*/
func str_user_input(topic string, clr Color, current_tc *ta.TestCase, save_point_device string) string {
	var user_input string
	if topic != "" {
		fmt.Println(clr, topic)
	}
	fmt.Print(user_input_sign, cyan)
	_, err := fmt.Scanln(&user_input)
	if err != nil && err.Error() != "unexpected newline" {
		colored_txt_output("Выход из программы", green)
		os.Exit(0)
	} else if err != nil {
		colored_txt_output(str_input_err, red)
	} else if user_input == help_test {
		help_menu(current_tc, save_point_device)
		colored_txt_output("Тестирование продолжается, введите результат предыдущего теста:", gray)
		user_input = str_user_input(topic, clr, current_tc, save_point_device)
	}
	return user_input
}

/*
Функция для строкового ввода с пробелами.
Возвращает введенную строку.
Topic - опциональная строка для отображения до пользовательского ввода.
*/
func write_problems(topic string) string {
	var reader = bufio.NewReader(os.Stdin)
	if topic != "" {
		fmt.Print(red, topic)
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
Функция для введения важности бага, посредством выбора из пяти вариантов.
Возвращает - строковое значение выбранной важности.
*/
func choose_bug_severity() string {
	colored_txt_output("Выберите важность найденного бага", blue)
	colored_txt_output("1. Тривиальный", blue)
	colored_txt_output("2. Низкий", blue)
	colored_txt_output("3. Высокий", blue)
	colored_txt_output("4. Критический", blue)
	colored_txt_output("5. Блокер", blue)
	input, err := int_user_input("Введите важность бага: ")
	if err != nil {
		colored_txt_output("Error reading int input", red)
		return ""
	} else {
		switch input {
		case 1:
			return ta.Trivial
		case 2:
			return ta.Minor
		case 3:
			return ta.High
		case 4:
			return ta.Critical
		case 5:
			return ta.Blocker
		default:
			return ""
		}
	}
}

/*
Функция для пользовательского ввода.
Обрабатывает внутренние ошибки.
*/
func int_user_input(topic string) (int, error) {
	var user_input string
	if topic != "" {
		fmt.Println(blue, topic)
	}
	colored_txt_output(user_input_sign, cReset)
	_, err := fmt.Scanln(&user_input, cyan)
	if err != nil && err.Error() == "unexpected newline" {
		colored_txt_output(int_input_err, red)
		return 0, err
	}
	return strconv.Atoi(user_input)
}

/*
Функция для создания массива статически заданной длины.
*/
func recreate_slice3() []ta.Test_result {
	return make([]ta.Test_result, tests_count)
}

/*
Функция для создания двойного массива повышенной длины.
*/
func recreate_double_slice(slice string) DoubleTestCaseContainer {
	return make(DoubleTestCaseContainer, len(slice)+2)
}
