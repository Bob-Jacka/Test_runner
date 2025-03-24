package Test_artifacts

import (
	"errors"
	"fmt"
)

// Константные обозначения полей тест кейса.
const (
	name_field     = "Name"
	priority_field = "Priority"
	severity_field = "Severity"
	msg_field      = "Msg"

	Undefined_field = "UNDEFINED" //Обозначение для полей тест кейса в случае ошибки.
)

// TestCase
// - Структура с данными для тест - кейса.
type TestCase struct {
	TestArtifact
	Name     string
	Priority string
	Severity string
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

func (tc TestCase) GetSeverity() string {
	if tc.Severity != "" && tc.Severity != Undefined_field {
		return tc.Severity
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

// Construct_test_case
// - Функция конструктор для тест кейса.
// Возвращает объект тест кейса и ошибку.
// Пример тест кейса - Нажать на кнопку ставки|Высокий|Высокая|"Проверить на разных скоростях интернета"
func Construct_test_case(inner_struct map[string]string) (TestCase, error) {
	if inner_struct != nil {
		return TestCase{
			Name:     inner_struct[name_field],
			Priority: inner_struct[priority_field],
			Severity: inner_struct[severity_field],
			Msg:      inner_struct[msg_field],
		}, nil
	} else {
		fmt.Println("Test case cannot be created.")
		return TestCase{
			Name:     Undefined_field,
			Priority: Undefined_field,
			Severity: Undefined_field,
			Msg:      Undefined_field,
		}, errors.New("test case missing of valuable parameters")
	}
}
