package Test_artifacts

import (
	"errors"
	"fmt"
)

// Константные обозначения полей тест кейса.
const (
	Name_field     = "Name"
	Priority_field = "Priority"
	Msg_field      = "Msg"

	Undefined_field = "UNDEFINED" //Обозначение для полей тест кейса в случае ошибки.
)

/*
TestCase
- Структура с данными для тест - кейса.
*/
type TestCase struct {
	ITestArtifact
	Name     string
	Priority string
	Msg      string
}

func (tc TestCase) GetName() string {
	if tc.Name != "" && tc.Name != Undefined_field {
		return tc.Name
	} else {
		return ""
	}
}

func (tc TestCase) GetPriority() string {
	if tc.Priority != "" && tc.Priority != Undefined_field {
		return tc.Priority
	} else {
		return ""
	}
}

func (tc TestCase) Print() {
	fmt.Println("Имя тест кейса: " + tc.Name)
}

func (tc TestCase) GetMessage() string {
	if tc.Msg != "" && tc.Msg != Undefined_field {
		return tc.Msg
	} else {
		return ""
	}
}

/*
	Construct_test_case
	- Функция конструктор для тест кейса.
	Возвращает объект тест кейса и ошибку.

Пример тест кейса - Нажать на кнопку ставки|Высокий|Высокая|"Проверить на разных скоростях интернета"
*/
func Construct_test_case(inner_struct map[string]string) (TestCase, error) {
	if inner_struct != nil {
		return TestCase{
			Name:     inner_struct[Name_field],
			Priority: inner_struct[Priority_field],
			Msg:      inner_struct[Msg_field],
		}, nil
	} else {
		fmt.Println("\tTest case cannot be created.")
		return TestCase{
			Name:     Undefined_field,
			Priority: Undefined_field,
			Msg:      Undefined_field,
		}, errors.New("test case missing of valuable parameters")
	}
}
