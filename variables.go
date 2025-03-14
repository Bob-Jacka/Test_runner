/*
Файл, использующийся для хранения типов в приложении для использования.
Содержаться как константные типы, так и изменяемые.
*/

package main

import (
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
			Используется для сохранения данных о тестировании.
	*/
	StringContainer = []string

	/*
		    Пользовательский тип для двойного массива строк.
			Используется для сохранения данных о тестировании.
	*/
	DoubleStringContainer = [][]string
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

	default_success_result = " - No errors"
	default_skipped_result = " - TEST SKIPPED"

	ignore_line_sign = "*" // Ignore line in test suit if it starts with this sign.
	nested_suit_sign = "$" // Represents line is test suit as another test suit if it starts with this sign.

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

	spend_time_on_test        = "Времени на тестирование ушло: "
	hours                     = "\tHours - "
	minutes                   = "\tMinutes - "
	seconds                   = "\tSeconds - "
	none_type          string = "None"

	other_info    = "Other info:"
	go_comp       = "\tGo compiler is "
	comp_arch     = "\tComputer arch at time is "
	max_processes = "\tMax available processors are "

	app_version = "2.5.2" // Версия приложения. Используется для вывода версии в help функции.
)

/*
Основные глобальные переменные.
*/
var (
	test_results DoubleStringContainer // Первый массив - объект тестирования, второй массив - результаты тестов каждого объекта.
	test_stages  StringContainer       // Этапы тестирования.

	devices_list StringContainer // Список, в котором содержатся устройства для тестирования.

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
