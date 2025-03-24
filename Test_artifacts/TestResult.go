package Test_artifacts

// Структура для результата теста.
type Test_result struct {
	TestArtifact
	Result  int
	Device  string
	Message string
}

func (tr Test_result) GetResult() int {
	if tr.Result != 0 {
		return tr.Result
	} else {
		return 0
	}
}

func (tr Test_result) GetDevice() string {
	if tr.Device != "" {
		return tr.Device
	} else {
		return ""
	}
}

func (tr Test_result) GetMessage() string {
	if tr.Message != "" {
		return tr.Message
	} else {
		return ""
	}
}

const (
	Skipped = iota
	Success
	Failed
)
