#ifndef LIST_H
#define LIST_H

#include <list>
#include <string>
#define ERROR_CODE 1

using namespace std;

template<typename T>
class List : public list<T> {
private:
    unsigned int __list_initial_size__ = 10;
    unsigned int __add_elements_to_size = 5;
    list<T> __inner_list__;

    /*
     * Method for resizing inner list, by __add_elements_to_size value. 
     */
    void resize_list() {
        unsigned int before_resize_size = __inner_list__.size();
        delete __inner_list__;
        __inner_list__ = new list(before_resize_size + this.__add_elements_to_size);
    }

public:
    /*
    List contructor.
    Needs initial size for construct.
    */
    List(unsigned int init_size) {
        __inner_list__ = list<T>{init_size};
    }

    /*
    List contructor.
    */
    List() {
        __inner_list__ = list<T>{__list_initial_size__};
    }

    /*
    List destructor.
    */
    ~List() {
        for (T elem : this->__inner_list__) {
            __inner_list__.remove(elem);
        }
        delete &__inner_list__;
    }

    /*
     Получение элемента по его позиции.
     */
    T getElement(unsigned int index) const {
        try {
            if (
                index >= __inner_list__.size() 
                &&
                __inner_list__.size() == 0
            ) {
            throw ERROR_CODE;
            }   
            auto it = __inner_list__.begin();
            std::advance(it, index);
            return *it;
        } catch (int code) {
            printf("error in list get elem %d\n", code);
            printf("List size is zero\n");
        }
    }

    /*
     *@return Размер списка 
     */
    [[nodiscard]] unsigned int getSize() const {
        return __inner_list__.size();
    }

    T getFront() {
        return __inner_list__.front();
    }

    T getLast() {
        return __inner_list__.back();
    }

    void setElement(unsigned int index, T value) {
        try {
            if (
                index >= __inner_list__.size() 
                &&
                __inner_list__. size() == 0
            ) {
            throw ERROR_CODE;
            }
            auto it = __inner_list__.begin();
            std::advance(it, index);
            *it = value;
        } catch (int code) {
            printf("error in set elem\n");
        }
    }

    /*
     Добавление элемента в конец списка
     */
    void addElement(T element) {
        __inner_list__.push_back(element);
    }

    void removeElement(const unsigned int index) {
        if (index >= __inner_list__.size()) {
            for (unsigned int i = 0; i < index; i++) {
                __inner_list__.erase(__inner_list__.begin());
            }
        }
    }

    void removeElement(T element) {
        __inner_list__.erase(__inner_list__.begin(), __inner_list__.find(element));
    }

    std::iterator<std::forward_iterator_tag, T> get_iterator() {
        return std::iterator<std::forward_iterator_tag, T>(__inner_list__.begin(), __inner_list__.end());
    }

    bool operator!=(const List &anotherL) const {
        return __inner_list__.getSize() != anotherL.getSize();
    }

    T operator[](const unsigned int index) const {
        return this->getElement(index);
    }

    bool operator==(const List &anotherL) const {
        return this->__inner_list__.size() == anotherL.size();
    }
};

inline string to_upper(const string &str) {
    const char *c = str.c_str();
    string ret;
    for (unsigned int i = 0; i < str.length(); i++) {
        ret.append(1, toupper(*c));
    }
    return ret;
}

/*
Returns List instance with size 1.
*/
inline List<string> get_empty_list() {
    return List<string>{1};
}

/*
Returns defauls List with size 10.
*/
inline List<string> default_list() {
    return List<string>{10};
}
#endif
