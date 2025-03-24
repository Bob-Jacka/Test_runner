/*
Файл, использующийся для хранения типов в приложении для использования.
Содержаться как константные типы, так и изменяемые.
*/

package main

import (
	ta "Test_runner_3-5-4/Test_artifacts"
	"fmt"
	"time"
)

// Определенные типы для программы.
type (

	/*
		    Пользовательский тип для цветов.
			Используется в цветном выводе в консоль.
	*/
	Color = string

	/*
		    Пользовательский тип для текущего времени.
			Используется для времени начала и конца тестирования.
	*/
	TimeType = time.Time

	/*
		    Пользовательский тип для массива строк.
			Используется для сохранения данных о устройств тестирования.
	*/
	StringContainer = []string

	/*
		    Пользовательский тип для массива строк.
			Используется для сохранения данных о тестировании.
	*/
	TestCaseContainer = []ta.TestCase

	/*
		Пользовательский тип для сохранения багов.
		Используется для сохранения данных о багах в тестировании.
	*/
	BugContainer = []ta.Bug

	/*
		    Пользовательский тип для двойного массива строк.
			Используется для сохранения данных о тестировании.
	*/
	DoubleTestCaseContainer = map[string][]ta.Test_result
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
	skip_test       = "skip"
	help_test       = "help"
	out_prog        = "out"

	default_success_result = " - No errors"
	default_skipped_result = " - TEST SKIPPED"

	ignore_line_sign     = "*" // Ignore line in test suit if it starts with this sign.
	nested_suit_sign     = "$" // Represents line is test suit as another test suit if it starts with this sign.
	multiline_test_sign  = "/" // Represents if test name consists of several lines.
	test_case_split_sign = "|" // Символ для разделения строки в тестовом наборе на тест кейс.

	fail_check_file     = "Error occurred in checking file."
	fail_on_write       = "Error occurred in writing file."
	fail_on_file_action = "Error occurred during file action."

	str_input_err      = "Error occurred in str_user_input."
	write_problems_err = "Error occurred in write_problems."
	int_input_err      = "Error occurred in int_user_input."

	file_name_ext = "results.txt"

	every_test_msg = "Enter (yes, 1) if success or (no, 0) for no if not or skip to skip."
	on_bug_msg     = "Напишите, что было не так: "
	on_save_msg    = "Saving in file."
	wrong_arg      = "Неправильный аргумент, попробуйте снова."

	spend_time_on_test        = "Времени на тестирование ушло: "
	hours                     = "\tЧасов - "
	minutes                   = "\tМинут - "
	seconds                   = "\tСекунд - "
	none_type          string = "None"

	other_info    = "Другая информация:"
	go_comp       = "\tКомпилятор Go - "
	comp_arch     = "\tКомпьютерная архитектура - "
	max_processes = "\tМаксимальное число процессоров - "

	app_version = "3.5.4" // Версия приложения. Используется для вывода версии в help функции.
)

/*
Основные глобальные переменные.
*/
var (
	test_results  DoubleTestCaseContainer // Карта - строка \ ключ. Ключ - объект тестирования, Значение - результаты тестов каждого объекта.
	test_stages   TestCaseContainer       // Массив тест-кейсов. Этапы тестирования.
	bug_container BugContainer
	devices_list  StringContainer // Список, в котором содержатся устройства для тестирования.

	start_time TimeType // Время начала тестирования.
	end_time   TimeType // Время конца тестирования.

	devices_count uint32 = 0 // Количество устройств на которых должно быть проведено тестирование.
	tests_count   uint32 = 0 // Количество тестов в тестовом наборе.
)

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
