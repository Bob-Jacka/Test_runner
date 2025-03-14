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
	"os"
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
		colored_txt_output("First cli argument is <test_suit>", blue)
		colored_txt_output("*Optional arg - Second cli argument is <devices_list>", blue)
		colored_txt_output("*Optional arg - Third cli argument is <true / false> - write results to file", blue)
		colored_txt_output("And optionally you can enter flag '-colored' to colored input in console", blue)
	case 1:
		object_tests(os.Args[1], none_type)
	case 2:
		object_tests(os.Args[1], os.Args[2])
		get_results(false)
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
*/
func object_tests(object_stages_cli string, object_devices_cli string) {
	start_time = get_now_time()

	test_stages = Atos(object_stages_cli, false)
	tests_count = uint32(len(test_stages))
	devices_list = Atos(object_devices_cli, true)
	if devices_count != 0 {
		test_results = recreate_double_slice(object_devices_cli)
	} else {
		colored_txt_output(test_res_error, red)
		os.Exit(1)
	}

	for device_num := 0; device_num < len(devices_list); device_num++ {
		colored_txt_output(to_upper(devices_list[device_num]), blue)
		suit_proceed(device_num)
	}

	end_time = get_now_time()
}

/*
Главная функция для тестирования.
Предоставляет тест из набора и запрашивает пользовательский ввод для результата.
Применяется для одного устройства.
*/
func suit_proceed(device_num int) {
	var stages_result = make(StringContainer, tests_count)
	for stage_num := 0; stage_num < len(test_stages); stage_num++ {
		colored_txt_output(to_upper(test_stages[stage_num]), white)
		colored_txt_output("\t"+every_test_msg, blue)

		var user_input = str_user_input("", green)
		var bool_res = reverse_scan(user_input)
		if bool_res == true {
			stages_result[stage_num] = test_stages[stage_num] + default_success_result
		} else if !bool_res && (user_input == skip_test || user_input == "") { // Был баг, если нажать перенос строки и затем валидный токен, то переносило в секцию багов.
			stages_result[stage_num] = test_stages[stage_num] + default_skipped_result
		} else if !bool_res {
			var problems = write_problems(on_bug_msg, red)
			stages_result[stage_num] = test_stages[stage_num] + " - " + problems
		}
	}
	test_results[device_num] = append(stages_result)
	stages_result = recreate_slice2()
}
