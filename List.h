#ifndef LIST_H
#define LIST_H
#include <list>

template<typename T>

class List : std::list<T> {
public:

    /*
     получение элемента по его позиции
     */
    T getElement(int index) {
        T element;
        if (index < 0 || index >= this->size()) {
            for (unsigned int i = 0; i < index; i++) {
                this->front();
            }
            element = this->front();
        }
        return element;
    }

    int getSize() {
        return this->size();
    }

    T getFront() {
        return this->front();
    }

    T getLast() {
        return this->back();
    }

    void setElement(int index, T element) {
        if (index < 0 || index >= this->size()) {
            for (unsigned int i = 0; i < index; i++) {
                this->insert(this->begin(), element); //TODO возможно проблема с insertom
            }
        }
    }

    /*
     добавление элемента в конец списка
     */
    void addElement(T element) {
        this->insert(this->end(), element);
    }
};
#endif //LIST_H
