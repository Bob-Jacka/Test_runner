package Test_artifacts

import (
	"fmt"
)

// Bug
// - Структура с данными для бага.
type Bug struct {
	TestArtifact
	Name     string
	Priority string
	Severity string
	Msg      string
}

func (b Bug) GetName() string {
	if b.Name != "" {
		return b.Name
	} else {
		return ""
	}
}

func (b Bug) GetPriority() string {
	if b.Priority != "" {
		return b.Priority
	} else {
		return ""
	}
}

func (b Bug) GetSeverity() string {
	if b.Severity != "" {
		return b.Severity
	} else {
		return ""
	}
}

func (b Bug) print() {
	fmt.Println("Имя бага: " + b.Name)
}

func (b Bug) GetMessage() string {
	if b.Msg != "" {
		return b.Msg
	} else {
		return ""
	}
}
