#ifndef LIST_H
#define LIST_H
#include <list>
#include <string>

template<typename T>

class List : public std::list<T> {
public:

    /*
     Получение элемента по его позиции
     */
    T getElement(const unsigned int index) const {
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

    void setElement(const unsigned int index, T element) {
        if (index >= this->size()) {
            for (unsigned int i = 0; i < index; i++) {
                this->insert(this->begin(), element);
            }
        }
    }

    /*
     Добавление элемента в конец списка
     */
    void addElement(T element) {
        this->insert(this->end(), element);
    }

    void removeElement(const unsigned int index) {
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

    bool operator!=(const List &anotherL) const {
        return this->getSize() != anotherL->getSize();
    }

    T operator[](const unsigned int index) const {
        return this->getElement(index);
    }

    bool operator==(const List &anotherL) const {
        return this->getSize() == anotherL->getSize();
    }

    std::ostream& operator<<(std::ostream& stream, List<T> input_list) {
        for(auto t : input_list) {
            stream << t;
        }
        return stream;
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
#endif
