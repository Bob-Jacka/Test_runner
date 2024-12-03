package main

import (
	"bufio"
	"fmt"
	"os"
	"runtime"
	"strings"
	"time"
)

type Color string

const (
	red    Color = "\033[31m"
	blue   Color = "\033[34m"
	green  Color = "\033[32m"
	yellow Color = "\033[33m"
	cReset Color = "\033[0m"
)

var test_results [][]string //первый массив - игра, второй массив - результаты тестов каждой игры

var test_stages []string
var games_list []string

var start_time time.Time
var end_time time.Time

var stage_count int

func main() {
	var args_len = len(os.Args)
	if args_len == 1 {
		fmt.Println("Utility usage:")
		fmt.Println("First cli argument is <Game stages>")
		fmt.Println("Second cli argument is <Devices>")
		fmt.Println("Third cli argument is <true / false write results to file>")
		fmt.Println()
		return
	} else if args_len == 3 {
		var hardcodeed_bool_value = "false"
		game_tests(os.Args[1], os.Args[2])
		get_results(Atob(hardcodeed_bool_value))
	} else if args_len == 4 {
		game_tests(os.Args[1], os.Args[2])
		get_results(Atob(os.Args[3]))
	} else {
		fmt.Println(red, "Given arguments - ")
		fmt.Print(+len(os.Args))
		fmt.Println(red, "Error in arguments")
		os.Exit(1)
	}
	defer fmt.Println(cReset, "Bye")
}

func game_tests(game_stages_cli string, games_devices_cli string) {
	start_time = time.Now()

	test_stages = Atos(game_stages_cli, false)
	games_list = Atos(games_devices_cli, true)
	if stage_count != 0 {
		test_results = make([][]string, len(games_devices_cli)+1)
	} else {
		printMSG("An error ocurred in making array test_results", red)
		os.Exit(1)
	}

	for game_num := 0; game_num < len(games_list); game_num++ {
		printMSG(strings.ToUpper(games_list[game_num]), yellow)
		game_stages(game_num)
	}
	end_time = time.Now()
}

func game_stages(game_num int) {
	var stages_result []string
	for stage := 0; stage < len(test_stages); stage++ {
		printMSG(strings.ToUpper(test_stages[stage]), yellow)
		printMSG("Enter 1 for yes if success or 0 for no if not or skip to skip", yellow)
		var txt string
		var _, error1 = fmt.Scan(&txt)
		if error1 != nil {
			printMSG("an error occured in scan game stage result", red)
			return
		}
		var res = reverse_scan(txt)
		newSlice := make([]string, len(stages_result)+1, cap(stages_result)+1*2)
		stages_result = newSlice
		if res == true {
			stages_result[stage] = test_stages[stage] + " No errors"
		} else if res == false && txt == "skip" {
			stages_result[stage] = test_stages[stage] + " TEST SKIPPED"
		} else if res == false {
			var problems string
			printMSG("Напишите, что было не так в тесте: ", red)
			var _, error2 = fmt.Scan(&problems)
			if error2 != nil {
				printMSG("an error occured in section about something bad in test", red)
				return
			}
			stages_result[stage] = test_stages[stage] + problems
		}
	}
	test_results[game_num] = append(stages_result)
}

func get_results(is_write_to_file bool) {
	if !is_write_to_file {
		print_results()
	} else {
		printMSG("Saving in file", green)
		write_to_file()
	}
}

func print_results() {
	fmt.Println()
	for game_num := 0; game_num < len(test_results); game_num++ {
		fmt.Println(cReset)
		fmt.Println(strings.ToUpper(games_list[game_num]))
		for stage := 0; stage < len(test_results[game_num]); stage++ {
			var res = test_results[game_num][stage]
			if res != "" {
				fmt.Println(blue, "\t"+res)
			}
		}
	}
	var dur = end_time.Sub(start_time)
	fmt.Println()
	fmt.Println(green, "Времени на тестирование ушло: ")
	fmt.Print(green, "\t Часов - ")
	fmt.Println(dur.Hours())
	fmt.Print(green, "\t Минут - ")
	fmt.Println(dur.Minutes())
	fmt.Print(green, "\t Секунд - ")
	fmt.Println(dur.Seconds())
	fmt.Println()
	fmt.Println(yellow, "Other info")
	fmt.Println(yellow, "Go compiler is "+runtime.Compiler)
	fmt.Println(yellow, "Computer arch at time is "+runtime.GOARCH)
	fmt.Print(yellow, "Max available processors are ")
	fmt.Println(runtime.GOMAXPROCS(runtime.NumCPU()))
}

func print_results_chan(to_write chan string) {
	fmt.Println()
	for game_num := 0; game_num < len(test_results); game_num++ {
		fmt.Println(cReset)
		fmt.Println(strings.ToUpper(games_list[game_num]))
		to_write <- strings.ToUpper(games_list[game_num])
		for stage := 0; stage < len(test_results[game_num]); stage++ {
			var res = test_results[game_num][stage]
			if res != "" {
				fmt.Println(blue, "\t"+res)
				to_write <- "\t" + res
			}
		}
	}
	var dur = end_time.Sub(start_time)
	fmt.Println()
	fmt.Println(green, "Времени на тестирование ушло: ")
	to_write <- "Времени на тестирование ушло: "

	fmt.Print(green, "\t Часов - ")
	to_write <- "\t Часов - "

	fmt.Println(dur.Hours())
	to_write <- string(rune(dur.Hours()))

	fmt.Print(green, "\t Минут - ")
	to_write <- "\t Минут - "

	fmt.Println(dur.Minutes())
	to_write <- string(rune(dur.Minutes()))

	fmt.Print(green, "\t Секунд - ")
	to_write <- "\t Секунд - "

	fmt.Println(dur.Seconds())
	to_write <- string(rune(dur.Seconds()))

	fmt.Println()
	fmt.Println(yellow, "Other info")
	to_write <- "Other info"

	fmt.Println(yellow, "Go compiler is "+runtime.Compiler)
	to_write <- "Go compiler is " + runtime.Compiler

	fmt.Println(yellow, "Computer arch at time is "+runtime.GOARCH)
	to_write <- "Computer arch at time is " + runtime.GOARCH

	fmt.Print(yellow, "Max available processors are ")
	to_write <- "Max available processors are "

	fmt.Println(runtime.GOMAXPROCS(runtime.NumCPU()))
	to_write <- string(rune(runtime.GOMAXPROCS(runtime.NumCPU())))
}

func write_to_file() {
	var file, err = os.Create("results.txt")
	var save_chan = make(chan string)
	if err != nil {
		printMSG("an Error ocurred while writing data to file", red)
		os.Exit(1)
		return
	} else {
		print_results_chan(save_chan)
	}
	var writer = bufio.NewWriter(file)
	for str := range save_chan {
		writer.WriteString(str)
	}
	defer file.Close()
}

func reverse_scan(scan_val string) bool {
	fmt.Print(green, ">> ")
	switch scan_val {
	case "yes", "y", "ye", "1":
		return true
	case "no", "n", "0":
		return false
	case "skip":
		return false
	default:
		printMSG("Invalid argument", red)
		printMSG("Please, try again", green)
		var txt string
		var _, err = fmt.Scan(&txt)
		if err != nil {
			printMSG("Error in recursion", red)
		}
		return reverse_scan(txt)
	}
}

/*
Проверка того, что переданный путь это файл
*/
func check_file(path string) bool {
	var _, err = os.Stat(path)
	if err != nil {
		printMSG("error occured in checking file", red)
		check_dir(path)
		if !strings.Contains(path, ".") {
			printMSG("Maybe path is not contains file extension", red)
		}
		os.Exit(1)
		return false
	} else {
		printMSG("File exists", green)
		return true
	}
}

/*
Проверка того, что переданный путь это директория
*/
func check_dir(path string) bool {
	var _, smth = os.ReadDir(path)
	if smth != nil {
		printMSG("error occured is checking dir", red)
		os.Exit(1)
		return false
	} else {
		printMSG("Dir exists", yellow)
		return true
	}
}

/*
Перевод строки в булево значение
*/
func Atob(str string) bool {
	switch str {
	case "true", "True", "1":
		return true
	case "false", "False", "0":
		return false
	default:
		printMSG("Invalid argument", red)
		os.Exit(1)
		return false
	}
}

/*
Перевод строки в массив
*/
func Atos(str string, increm bool) []string {
	if check_file(str) {
		printMSG("Argument is a file", green)
		return proceed_file(str, increm)
	} else if strings.Contains(str, ",") {
		printMSG("using as separator ','", green)
		var splitted = strings.Split(str, ",")
		return splitted
	} else {
		printMSG("using as separator ' ' *whitespace", red)
		var splitted = strings.Split(str, " ")
		return splitted
	}
}

/*
Функция выполняет открытие файла и читает его содержимое (построчно)
Возвращается содержимое файла
*/
func proceed_file(path string, increm bool) []string {
	var file, err = os.Open(path)
	if err != nil {
		printMSG("error ocurred during file open", red)
		file.Close()
		os.Exit(1)
	}
	var readed []string
	var reader = bufio.NewReader(file)
	var scanner = bufio.NewScanner(reader)

	for scanner.Scan() {
		line := scanner.Text() //строка
		if len(line) > 0 {
			readed = append(readed, line)
			if increm {
				stage_count++
			}
		}
	}
	defer file.Close()
	return readed
}

/*
Вывод сообщения независимо от цвета
*/
func printMSG(str string, clr Color) {
	fmt.Print(cReset)
	fmt.Println(clr, str)
	fmt.Print(cReset)
}
