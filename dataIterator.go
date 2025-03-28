package main

import (
	"iter"
	"math/rand"
)

/*
Тип оболочка для встроенного в Go итератора.
*/
type Iterator[V any] struct {
	iter iter.Seq[V]
}

/*
Метод для завершения цепочки вызовов.
Возвращает текущий объект контейнера.
Терминальный метод.
*/
func (i Iterator[V]) Collect() []V {
	collect := make([]V, 0)
	for e := range i.iter {
		collect = append(collect, e)
	}
	return collect
}

/*
Функция для создания итератора из приведенного значения.
*/
func From[V any](slice []V) *Iterator[V] {
	return &Iterator[V]{
		iter: func(yield func(V) bool) {
			for _, v := range slice {
				if !yield(v) {
					return
				}
			}
		},
	}
}

/*
Метод для применения функции к каждому элементу контейнера.
Принимает - функция для применения к каждому элементу.
Терминальный метод.
*/
func (i *Iterator[V]) Each(f func(V)) {
	for i := range i.iter {
		f(i)
	}
}

/*
Метод для переворачивания элементов в контейнере.
Возвращает - текущий объект итератора.
*/
func (i *Iterator[V]) Reverse() *Iterator[V] {
	collected := i.Collect()
	counter := len(collected) - 1
	for e := range i.iter {
		collected[counter] = e
		counter--
	}
	return From(collected)
}

/*
Метод для псевдо рандомного запуска тестов в контейнере.
Возвращает - текущий объект итератора.
*/
func (i *Iterator[V]) Random() *Iterator[V] {
	collect := i.Collect()
	var counter int
	for e := range i.iter {
		counter = rand.Intn(len(collect))
		collect[counter] = e
	}
	return From(collect)
}

/*
Метод для применения функции к каждому элементу контейнеру.
Принимает - функция для применения.
Возвращает - текущий объект итератора.
*/
func (i *Iterator[V]) Map(f func(V) V) *Iterator[V] {
	copy_iter := i.iter
	i.iter = func(yield func(V) bool) {
		for v := range copy_iter {
			v = f(v)
			if !yield(v) {
				return
			}
		}
	}
	return i
}

/*
Метод для фильтрации элементов в контейнере по заданной функции.
Принимает - функция для фильтрации.
Возвращает - текущий объект итератора.
*/
func (i *Iterator[V]) Filter(f func(V) bool) *Iterator[V] {
	copy_iter := i.iter
	i.iter = func(yield func(V) bool) {
		for v := range copy_iter {
			if f(v) {
				if !yield(v) {
					return
				}
			}
		}
	}
	return i
}
