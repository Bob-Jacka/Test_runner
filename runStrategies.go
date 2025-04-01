package main

import (
	ta "Test_runner/Test_artifacts"
)

// Run strategies type.
type (
	Strategy[V any] interface {
		run(iterator Iterator[V]) []V
	}

	/*
		Стратегия для рандомного "прогона" тестов.
	*/
	Random_run_strategy struct {
	}

	/*
		Стратегия для "прогона" только высокоприоритетных тестов.
	*/
	Only_high_prior struct {
	}

	/*
		Стратегия с обычным прогоном тестов.
	*/
	Usual_strategy struct {
	}
)

/*
Запуск тестов в псевдо-рандомном порядке.
Iterator - итератор для применения стратегии.
Возвращает - измененный набор тестов.
*/
func (r Random_run_strategy) run(iterator Iterator[ta.TestCase]) []ta.TestCase {
	colored_txt_output("Используется стратегия псевдо-случайного запуска тестов.", blue)
	return iterator.Random().Collect()
}

/*
Запуск только высокоприоритетных тестов.
Iterator - итератор для применения стратегии.
Возвращает - измененный набор тестов.
*/
func (r Only_high_prior) run(iterator Iterator[ta.TestCase]) []ta.TestCase {
	colored_txt_output("Используется стратегия запуска высокоприоритетных тестов.", blue)
	return iterator.Filter(func(testCase ta.TestCase) bool {
		if testCase.Priority == ta.High_prior || testCase.Priority == ta.As_soon_as_possible {
			return true
		}
		return false
	}).Collect()
}

/*
Запуск тестов в обычном порядке.
*Применяется по дефолту.
Iterator - итератор для применения стратегии.
Возвращает - измененный набор тестов.
*/
func (r Usual_strategy) run(iterator Iterator[ta.TestCase]) []ta.TestCase {
	colored_txt_output("Используется стратегия обычного запуска тестов.", blue)
	return iterator.Collect()
}

/*
Функция для выбора стратегии относительно загрузочных параметров.
*/
func strategy_factory() Strategy[ta.TestCase] {
	if load_parameters.High_prior {
		return Only_high_prior{}
	} else if load_parameters.Random_run {
		return Random_run_strategy{}
	} else if !load_parameters.High_prior {
		return Usual_strategy{}
	}
	return nil
}
