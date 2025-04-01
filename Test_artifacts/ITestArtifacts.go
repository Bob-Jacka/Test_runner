package Test_artifacts

/*
ITestArtifact - Интерфейс для тестовых артефактов.
*/
type ITestArtifact interface {

	//Метод для "распечатывания" объекта.
	print()

	//Метод для получения имени.
	//Возвращает строку.
	getName() string

	//Метод для получения приоритета.
	//Возвращает строку.
	getPriority() string

	//Метод для получения "важности".
	//Возвращает строку.
	getSeverity() string

	//Метод для получения сообщения.
	//Возвращает строку.
	getMessage() string
}

// Список показателей важности бага.
const (
	Trivial  = "Тривиальный" //Опечатка в тексте.
	Minor    = "Низкий"      //Небольшой баг с отображением.
	High     = "Высокий"     // Не работает незначительная функциональность (есть обходной путь).
	Critical = "Критический" // Не работает значительная функциональность (есть обходной путь).
	Blocker  = "Блокер"      //Ничего не работает...
)

// Список показателей приоритета бага или теста.
const (
	Low                 = "Низкий"           //Когда-нибудь исправить.
	Medium              = "Средний"          //Исправить скорее.
	High_prior          = "Высокий"          //Исправить быстрее, чем среднее.
	As_soon_as_possible = "Как можно скорее" //Обычно для очень критичных багов, необходимо исправить как можно скорее.
)
