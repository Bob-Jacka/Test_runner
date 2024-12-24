#ifndef LIST_H
#define LIST_H
#include <list>
#include <string>

template<typename T>

class List : public std::list<T> {
public:

    /*
     получение элемента по его позиции
     */
    T getElement(unsigned int index) const {
        T element;
        if (index >= this->size()) {
            for (unsigned int i = 0; i < index; i++) {
                this->front();
            }
            element = this->front();
        }
        return element;
    }

    [[nodiscard]] unsigned int getSize() const {
        return this->size();
    }

    T getFront() {
        return this->front();
    }

    T getLast() {
        return this->back();
    }

    void setElement(unsigned int index, T element) {
        if (index >= this->size()) {
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

    void removeElement(unsigned int index) {
        if (index >= this->size()) {
            for (unsigned int i = 0; i < index; i++) {
                this->erase(this->begin());
            }
        }
    }

    void removeElement(T element) {
        this->erase(this->begin(), this->find(element));
    }

    std::iterator<std::forward_iterator_tag, T> get_iterator() {
        return std::iterator<std::forward_iterator_tag, T>(this->begin(), this->end());
    }

    bool operator!=(const List &) const {
        return false; //TODO будет ошибка
    }
};

inline std::string to_upper(const std::string &str) {
    const char *c = str.c_str();
    std::string ret;
    for (unsigned int i = 0; i < str.length(); i++) {
        ret.append(1, toupper(*c));
    }
    return ret;
}
#endif //LIST_H
