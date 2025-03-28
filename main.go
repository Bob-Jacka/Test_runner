/*
				Test runner.
Program for testing, written in GO language v1.23.2.

author - cupcake_WRLD

Test runner architecture:
                             (Program entry point)
									main
                                 Atos | load_parameters
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
	ta "Test_runner/Test_artifacts"
	"os"
	"strconv"
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
		colored_txt_output("Утилита предполагает только использование флагов для передачи параметров,", blue)
		colored_txt_output("В качестве валидных флагов можно использовать такие слова, как:", blue)
		colored_txt_output("\t'suit' - флаг (строка) для передачи набора тестов (*обязательный),", blue)
		colored_txt_output("\t'devices' - флаг (строка) для передачи набора устройств для тестирования (*необязательный),", blue)
		colored_txt_output("\t'file_save' - флаг (булево значение) для сохранения в файл (*необязательный),", blue)
		colored_txt_output("\t'file_load' - флаг (булево значение) для загрузки из файла (*необязательный),", blue)
		colored_txt_output("\t'tc_time' - флаг (булево значение) для отображения времени выполнения тест кейса (*необязательный),", blue)
		colored_txt_output("\t'high_prior' - флаг (булево значение) для прогона тестов только с высоким приоритетом (*необязательный),", blue)
		colored_txt_output("\t'random_run' - флаг (булево значение) для прогона тестов в случайном порядке (*необязательный),", blue)
		colored_txt_output("\t'comments' - флаг (булево значение) для отображения внутреннего сообщения тест кейса, если есть (*необязательный),", blue)
		colored_txt_output("\t'colored' - флаг (булево значение) для цветного вывода сообщений (*необязательный).", blue)
		colored_txt_output("", blue)
		colored_txt_output("Пример заполнения тест кейса для приложения:", blue)
		colored_txt_output("\t<Название-тест-кейса>|<Приоритет/Priority>|<Сообщение/MSG>", blue)
		colored_txt_output("Пример:", blue)
		colored_txt_output("\tВыключить интернет во время раунда|Как можно скорее|необходимо выключить интернет после начала раунда игры", blue)
		colored_txt_output("\tВыключить интернет во время раунда|Средний|", blue)
		colored_txt_output("\tВыключить интернет во время раунда|Как можно скорее|", blue)
		colored_txt_output("\tВыключить интернет во время раунда||", blue)
		colored_txt_output("Параметры приоритет, важность и сообщение могут быть нулевыми и не указываться.", blue)
		colored_txt_output("\t'|' - символ разделения должен обязательно присутствовать в конце тест кейса (не набора), иначе будет ошибка.", blue)
		colored_txt_output("", blue)
		colored_txt_output("Заполнение набора тест кейсов:", blue)
		colored_txt_output("\t"+ignore_line_sign+" - символ в начале строки в наборе игнорирует символы за ним,", blue)
		colored_txt_output("\t"+nested_suit_sign+" - символ в начале строки в наборе представляет собой вложенный набор тестов,", blue)
		colored_txt_output("Валидные значения для приоритета:", blue)
		colored_txt_output("\t"+ta.Low, blue)
		colored_txt_output("\t"+ta.Medium, blue)
		colored_txt_output("\t"+ta.As_soon_as_possible, blue)
		colored_txt_output("Опционально - во время тестирования можно применять слова 'exit', 'help' для дополнительных действий.", blue)
	case 1, 2, 3, 4, 5, 6, 7, 8, 9:
		for _, param := range os.Args[1:] {
			check_flag(param)
		}
		object_tests(&load_parameters)
		get_results(load_parameters.File_save)
	}
	defer func() {
		colored_txt_output("Bye!", magenta)
		os.Exit(666)
	}()
}

/*
Функция для проверки флагов командной строки.
*/
func check_flag(str_to_check string) {
	var check_func_full = func(str string) bool {
		return strings.Contains(str, "=") && (strings.HasPrefix(str, "--") || strings.HasPrefix(str, "-"))
	}
	if check_func_full(str_to_check) {
		var split_line = strings.Split(str_to_check, "=")
		var flag_name = strings.ToLower(strings.ReplaceAll(split_line[0], "-", ""))
		var flag_value = split_line[1]
		switch flag_name {
		case "suit":
			load_parameters.Suit = flag_value
			break
		case "devices":
			load_parameters.Devices = flag_value
			break
		case "file_save":
			load_parameters.File_save, _ = strconv.ParseBool(flag_value)
			break
		case "file_load":
			load_parameters.File_load, _ = strconv.ParseBool(flag_value)
			break
		case "tc_time":
			load_parameters.Tc_time, _ = strconv.ParseBool(flag_value)
			break
		case "high_prior":
			load_parameters.High_prior, _ = strconv.ParseBool(flag_value)
			break
		case "random_run":
			load_parameters.Random_run, _ = strconv.ParseBool(flag_value)
			break
		case "comments":
			load_parameters.Comments, _ = strconv.ParseBool(flag_value)
			break
		case "colored":
			load_parameters.Colored, _ = strconv.ParseBool(flag_value)
			break
		default:
			colored_txt_output("Unknown flag: "+flag_name, red)
			os.Exit(1)
		}
	} else {
		colored_txt_output("Error occurred in check_flag func.", red)
		os.Exit(1)
	}
}

/*
Более общая функция для тестирования.
Комбинирует устройства с тестовыми наборами и проводит для каждого устройства тестовый набор.
Object_stages_cli - строка, которая будет использоваться для этапов тестирования.
Object_devices_cli - строка, которая будет использоваться для объектов тестирования.
*/
func object_tests(params *Load_params) {
	start_time = get_now_time()

	test_stages = Atos(params.Suit)
	tests_count = uint32(len(test_stages))
	{ // Блок инициализации количества устройств.
		if params.Devices != "" {
			devices_list = *get_devices(params.Devices)
		} else {
			devices_list = *get_devices(none_type)
		}
		if devices_count != 0 {
			test_results = recreate_double_slice(params.Devices)
		} else {
			colored_txt_output("Devices count cannot be 0.", red)
			os.Exit(1)
		}
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
	var test_stages_iter = From(test_stages)
	var list_changed_with_strategy = strategy_factory().run(*test_stages_iter) // объект измененного стратегией набора.

	for stage_num := 0; stage_num < len(list_changed_with_strategy); stage_num++ {
		var start_test TimeType //Начальное время тест кейса, применяется только если установлен флаг tc_time.
		var end_test TimeType   //Конечное время тест кейса, применяется только если установлен флаг tc_time.
		var current_test_case = list_changed_with_strategy[stage_num]

		if current_test_case.Name != ta.Undefined_field {
			colored_txt_output(to_upper(current_test_case.Name), white)
			if load_parameters.Comments && current_test_case.Msg != "" {
				colored_txt_output("\tСообщение - "+current_test_case.Msg, blue)
			}
			colored_txt_output("\t"+every_test_msg, blue)

			if load_parameters.Tc_time {
				start_test = get_now_time()
			}
			var user_input = str_user_input("", green, &current_test_case, device_name)
			var bool_res = reverse_scan(user_input)

			if user_input == out_prog && bool_res == false {
				goto outerLabel //Выход из игры
			} else {
				if bool_res == true {
					stages_result[stage_num] = ta.Test_result{
						Result:  ta.Success,
						Device:  device_name,
						Message: list_changed_with_strategy[stage_num].Name + default_success_result,
					}

				} else if !bool_res && (user_input == skip_test || user_input == "") {
					stages_result[stage_num] = ta.Test_result{
						Result:  ta.Skipped,
						Device:  device_name,
						Message: list_changed_with_strategy[stage_num].Name + default_skipped_result,
					}

				} else {
					var problems = write_problems(on_bug_msg)
					var bug = ta.Bug{
						Name:     write_problems("Введите название бага: "),
						Priority: write_problems("Введите приоритет бага: "),
						Severity: choose_bug_severity(),
						Msg:      problems,
					}
					bug_container = append(bug_container, bug)
					stages_result[stage_num] = ta.Test_result{
						Result:  ta.Failed,
						Device:  device_name,
						Message: list_changed_with_strategy[stage_num].Name + " - " + problems,
					}
				}
			}
		} else {
			colored_txt_output("Empty or wrong test case found.", red)
		}
		if load_parameters.Tc_time {
			end_test = get_now_time()
			colored_txt_output("\tТест занял - "+strconv.FormatInt(int64(end_test.Sub(start_test).Seconds()), 5)+" секунд.", blue)
		}
	}
	test_results[device_name] = append(stages_result)
	stages_result = recreate_slice3()
outerLabel:
}
