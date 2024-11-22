package main

import (
	"fmt"
	"os"
	"runtime"
	"strings"
	"time"
)

const (
	first_stage  = "originals desktop"
	second_stage = "originals mobile android"
	third_stage  = "originals mobile ios"
	fourth_stage = "casual desktop"
	fifth_stage  = "casual mobile android"
	sixth_stage  = "casual mobile ios"

	first_game  = "mooscape"
	second_game = "lucky_fish"
	third_game  = "color_race"
	fourth_game = "pirate flipping coins"

	colorRed    = "\033[31m"
	colorBlue   = "\033[34m"
	colorGreen  = "\033[32m"
	colorYellow = "\033[33m"

	colorReset = "\033[0m"
)

var test_results [4][]string //первый массив - игра, второй массив - результаты

var test_stages [6]string
var games [4]string

var start_time time.Time
var end_time time.Time

func main() {
	var args = len(os.Args)
	if args == 1 {
		fmt.Println("Utility usage:")
		fmt.Println("first cli argument is <Game stages>")
		fmt.Println("second cli argument is <Games list>")
		fmt.Println("third cli argument is <true / false write results to file>")
		return
	} else if args == 3 {
		game_tests(os.Args[2], os.Args[3])
		get_results(STtoB(os.Args[4]))
	}
}

func game_tests(game_stages_cli string, games_list_cli string) {
	test_stages[0] = first_stage
	test_stages[1] = second_stage
	test_stages[2] = third_stage
	test_stages[3] = fourth_stage
	test_stages[4] = fifth_stage
	test_stages[5] = sixth_stage

	games[0] = first_game
	games[1] = second_game
	games[2] = third_game
	games[3] = fourth_game
	start_time = time.Now()
	for game_num := 0; game_num < len(games); game_num++ {
		fmt.Println()
		fmt.Println(strings.ToUpper(games[game_num]))
		game_stages(game_num)
	}
	end_time = time.Now()
}

func game_stages(game_num int) {
	var stages_result = []string{"", "", "", "", "", ""}
	for stage := 0; stage < len(test_stages); stage++ {
		fmt.Println(colorYellow, strings.ToUpper(test_stages[stage]))
		fmt.Println(colorReset, "Enter 1 for yes if success or 0 for no if not or skip to skip")
		var txt string
		var _, error1 = fmt.Scan(&txt)
		if error1 != nil {
			fmt.Println(colorRed, "an error occured in scan game stage result")
			fmt.Println(colorReset)
			return
		}
		var res = reverse_scan(txt)
		if res == true {
			stages_result[stage] = test_stages[stage] + " no errors"
		} else if res == false && txt == "skip" {
			stages_result[stage] = test_stages[stage] + " TEST SKIPPED"
		} else if res == false {
			var problems string
			fmt.Println("Напишите, что было не так в тесте: ")
			var _, error2 = fmt.Scan(&problems)
			if error2 != nil {
				fmt.Println(colorRed, "an error occured in section about something bad in test")
				fmt.Println(colorReset)
				return
			}
			stages_result[stage] = test_stages[stage] + problems
		}
	}
	test_results[game_num] = append(stages_result)
}

func print_results() {
	fmt.Println()
	for game_num := 0; game_num < len(test_results); game_num++ {
		fmt.Println(colorReset)
		fmt.Println(strings.ToUpper(games[game_num]))
		for stage := 0; stage < len(test_results[game_num]); stage++ {
			var res = test_results[game_num][stage]
			if res != "" {
				fmt.Println(colorBlue, "\t"+res)
			}
		}
	}
	var dur = end_time.Sub(start_time)
	fmt.Println()
	fmt.Println(colorGreen, "Времени на тестирование ушло: ")
	fmt.Print(colorGreen, "\t Часов - ")
	fmt.Println(dur.Hours())
	fmt.Print(colorGreen, "\t Минут - ")
	fmt.Println(dur.Minutes())
	fmt.Print(colorGreen, "\t Секунд - ")
	fmt.Println(dur.Seconds())
	fmt.Println()
	fmt.Println(colorYellow, "Other info")
	fmt.Println(colorYellow, "Go compiler is "+runtime.Compiler)
	fmt.Println(colorYellow, "Computer arch at time is "+runtime.GOARCH)
	fmt.Print(colorYellow, "Max available processors are ")
	fmt.Println(runtime.GOMAXPROCS(runtime.NumCPU()))
}

func get_results(is_write_to_file bool) {
	if !is_write_to_file {
		print_results()
	} else {

	}
}

func reverse_scan(scan_val string) bool {
	switch scan_val {
	case "yes", "y", "ye", "1":
		return true
	case "no", "n", "0":
		return false
	case "skip":
		return false
	}
	fmt.Println(colorRed, "Invalid argument")
	defer os.Exit(1)
	return false
}

func check_dir(str string) bool {
	var _, smth = os.ReadDir(str)
	if smth != nil {
		fmt.Println(colorRed, "error occured")
		os.Exit(1)
		return false
	} else {
		return true
	}
}

func STtoB(str string) bool {
	switch str {
	case "true", "True":
		return true
	case "false", "False":
		return false
	default:
		fmt.Println(colorRed, "Invalid argument")
		os.Exit(1)
		return false
	}
}
