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

Правила:
1) Функции, содержащие несколько слов должны называться с маленькой буквы с содержать нижнее подчеркивание для разделения слов.
2) Методы для типов называются с помощью camelCase.
3) Пользовательские типы также пишутся с помощью camelCase.
*/

package main

import (
	ta "Test_runner_3-5-3/Test_artifacts"
	"os"
	"strings"
)

/*
Точка входа в программу.
Принимает аргументы консоли и на их основании делает дальнейшее выполнение.
*/
func main() {
	var args_len = len(os.Args) - 1 // минус 1, потому что 1-й аргумент это имя файла.
	switch args_len {
	case 0:
		colored_txt_output("Использование утилиты:", blue)
		colored_txt_output("Первый аргумент командной строки - <тестовые наборы>", blue)
		colored_txt_output("*Optional arg - Второй аргумент командной строки - <список устройств>", blue)
		colored_txt_output("*Optional arg - Третий аргумент командной строки - <true / false> - write results to file", blue)
	case 1:
		object_tests(os.Args[1], none_type)
	case 2:
		var sec_value = Atob(os.Args[2])
		if sec_value {
			object_tests(os.Args[1], none_type)
			get_results(sec_value)
		} else {
			object_tests(os.Args[1], os.Args[2])
			get_results(false)
		}
	case 3:
		object_tests(os.Args[1], os.Args[2])
		get_results(Atob(os.Args[3]))
	}
	defer func() {
		colored_txt_output("Bye!", magenta)
		os.Exit(666)
	}()
}

/*
Более общая функция для тестирования.
Комбинирует устройства с тестовыми наборами и проводит для каждого устройства тестовый набор.
Object_stages_cli - строка, которая будет использоваться для этапов тестирования.
Object_devices_cli - строка, которая будет использоваться для объектов тестирования.
*/
func object_tests(object_stages_cli string, object_devices_cli string) {
	start_time = get_now_time()

	test_stages = Atos(object_stages_cli)
	tests_count = uint32(len(test_stages))
	devices_list = get_devices(object_devices_cli)
	if devices_count != 0 {
		test_results = recreate_double_slice(object_devices_cli)
	} else {
		colored_txt_output(test_res_error, red)
		os.Exit(1)
	}

	for _, device := range devices_list {
		colored_txt_output(to_upper(device), blue)
		suit_proceed(device)
	}

	end_time = get_now_time()
}

/*
Главная функция для тестирования.
Предоставляет тест из набора и запрашивает пользовательский ввод для результата.
Применяется для одного устройства.
Device_name - имя устройства, на котором происходит тестирование.
*/
func suit_proceed(device_name string) {
	var stages_result = make([]ta.Test_result, tests_count)
outerLabel:
	for stage_num := 0; stage_num < len(test_stages); stage_num++ {
		var current_test_case = test_stages[stage_num]

		if strings.Compare(current_test_case.GetName(), ta.Undefined_field) != -1 {
			colored_txt_output(to_upper(current_test_case.GetName()), white)
			colored_txt_output("\t"+every_test_msg, blue)

			var user_input = str_user_input("", green, current_test_case.Name, device_name)
			var bool_res = reverse_scan(user_input)

			if user_input == out_prog && bool_res == false {
				break outerLabel
			} else {
				if bool_res == true {
					stages_result[stage_num] = ta.Test_result{
						Result:  ta.Success,
						Device:  device_name,
						Message: test_stages[stage_num].Name + default_success_result,
					}

				} else if !bool_res && (user_input == skip_test || user_input == "") {
					stages_result[stage_num] = ta.Test_result{
						Result:  ta.Skipped,
						Device:  device_name,
						Message: test_stages[stage_num].Name + default_skipped_result,
					}

				} else if !bool_res {
					var problems = write_problems(on_bug_msg)
					var bug = ta.Bug{
						Name:     write_problems("Введите название бага: "),
						Priority: write_problems("Введите приоритет бага: "),
						Severity: write_problems("Введите важность бага: "),
						Msg:      problems,
					}
					bug_container = append(bug_container, bug)
					stages_result[stage_num] = ta.Test_result{
						Result:  ta.Failed,
						Device:  device_name,
						Message: test_stages[stage_num].Name + " - " + problems,
					}
				}
			}
		} else {
			colored_txt_output("Пустой или невалидный тест кейс обнаружен", red)
		}
	}
	test_results[device_name] = append(stages_result)
	stages_result = recreate_slice3()
}
