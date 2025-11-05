#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
public:
    class const_iterator;
    class iterator;

protected:
    class node {
    public:
        T data;
        node *prev;
        node *next;

        node() : prev(nullptr), next(nullptr) {}
        node(const T &value) : data(value), prev(nullptr), next(nullptr) {}
        ~node() {}
    };

protected:
    node *head;
    node *tail;
    size_t list_size;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        list_size++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        list_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *current;
        const list *container;

    public:
        friend class list<T>;
        friend class const_iterator;
        iterator() : current(nullptr), container(nullptr) {}
        iterator(node *n, const list *c) : current(n), container(c) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            if (current == nullptr || current == container->tail) {
                throw invalid_iterator();
            }
            iterator temp = *this;
            current = current->next;
            return temp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (current == nullptr || current == container->tail) {
                throw invalid_iterator();
            }
            current = current->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            if (current == nullptr || current == container->head->next) {
                throw invalid_iterator();
            }
            iterator temp = *this;
            current = current->prev;
            return temp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (current == nullptr || current == container->head->next) {
                throw invalid_iterator();
            }
            current = current->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (current == nullptr || current == container->head || current == container->tail) {
                throw invalid_iterator();
            }
            return current->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (current == nullptr || current == container->head || current == container->tail) {
                throw invalid_iterator();
            }
            return &(current->data);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return current == rhs.current;
        }
        bool operator==(const const_iterator &rhs) const {
            return current == rhs.current;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return current != rhs.current;
        }
        bool operator!=(const const_iterator &rhs) const {
            return current != rhs.current;
        }
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *current;
        const list *container;

    public:
        friend class list<T>;
        friend class iterator;
        const_iterator() : current(nullptr), container(nullptr) {}
        const_iterator(const node *n, const list *c) : current(n), container(c) {}
        const_iterator(const iterator &it) : current(it.current), container(it.container) {}

        /**
         * iter++
         */
        const_iterator operator++(int) {
            if (current == nullptr || current == container->tail) {
                throw invalid_iterator();
            }
            const_iterator temp = *this;
            current = current->next;
            return temp;
        }
        /**
         * ++iter
         */
        const_iterator & operator++() {
            if (current == nullptr || current == container->tail) {
                throw invalid_iterator();
            }
            current = current->next;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            if (current == nullptr || current == container->head->next) {
                throw invalid_iterator();
            }
            const_iterator temp = *this;
            current = current->prev;
            return temp;
        }
        /**
         * --iter
         */
        const_iterator & operator--() {
            if (current == nullptr || current == container->head->next) {
                throw invalid_iterator();
            }
            current = current->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        const T & operator *() const {
            if (current == nullptr || current == container->head || current == container->tail) {
                throw invalid_iterator();
            }
            return current->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        const T * operator ->() const {
            if (current == nullptr || current == container->head || current == container->tail) {
                throw invalid_iterator();
            }
            return &(current->data);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const const_iterator &rhs) const {
            return current == rhs.current;
        }
        bool operator==(const iterator &rhs) const {
            return current == rhs.current;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const const_iterator &rhs) const {
            return current != rhs.current;
        }
        bool operator!=(const iterator &rhs) const {
            return current != rhs.current;
        }
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        // Create sentinel nodes without calling T's constructor
        head = static_cast<node*>(::operator new(sizeof(node)));
        tail = static_cast<node*>(::operator new(sizeof(node)));
        head->prev = nullptr;
        head->next = tail;
        tail->prev = head;
        tail->next = nullptr;
        list_size = 0;
    }
    list(const list &other) {
        // Create sentinel nodes without calling T's constructor
        head = static_cast<node*>(::operator new(sizeof(node)));
        tail = static_cast<node*>(::operator new(sizeof(node)));
        head->prev = nullptr;
        head->next = tail;
        tail->prev = head;
        tail->next = nullptr;
        list_size = 0;

        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
    }
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        ::operator delete(head);
        ::operator delete(tail);
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;

        clear();
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) throw container_is_empty();
        return head->next->data;
    }
    const T & back() const {
        if (empty()) throw container_is_empty();
        return tail->prev->data;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return list_size == 0;
    }
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return list_size;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (!empty()) {
            pop_front();
        }
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) throw invalid_iterator();

        node *new_node = new node(value);
        node *pos_node = pos.current;

        insert(pos_node, new_node);
        return iterator(new_node, this);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) throw container_is_empty();
        if (pos.container != this || pos == end()) throw invalid_iterator();

        node *pos_node = pos.current;
        node *next_node = pos_node->next;

        erase(pos_node);
        delete pos_node;

        return iterator(next_node, this);
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *new_node = new node(value);
        insert(tail, new_node);
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();
        node *last_node = tail->prev;
        erase(last_node);
        delete last_node;
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *new_node = new node(value);
        insert(head->next, new_node);
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();
        node *first_node = head->next;
        erase(first_node);
        delete first_node;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (size() <= 1) return;

        // Allocate raw memory for array
        size_t n = size();
        void *raw_memory = ::operator new(n * sizeof(T));
        T *arr = static_cast<T*>(raw_memory);

        // Copy elements to array using placement new
        size_t i = 0;
        for (iterator it = begin(); it != end(); ++it) {
            new (&arr[i]) T(*it);
            i++;
        }

        // Use provided sort function with operator<
        std::function<bool(const T&, const T&)> cmp = [](const T &a, const T &b) { return a < b; };
        sjtu::sort(arr, arr + n, cmp);

        // Copy back to list and destroy array elements
        i = 0;
        for (iterator it = begin(); it != end(); ++it) {
            *it = arr[i];
            arr[i].~T();
            i++;
        }

        // Free raw memory
        ::operator delete(raw_memory);
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;

        node *this_ptr = head->next;
        node *other_ptr = other.head->next;

        while (this_ptr != tail && other_ptr != other.tail) {
            if (other_ptr->data < this_ptr->data) {
                // Insert other node before this node
                node *next_other = other_ptr->next;

                // Remove from other list
                other.erase(other_ptr);

                // Insert into this list
                insert(this_ptr, other_ptr);

                other_ptr = next_other;
            } else {
                this_ptr = this_ptr->next;
            }
        }

        // Append remaining elements from other
        while (other_ptr != other.tail) {
            node *next_other = other_ptr->next;
            other.erase(other_ptr);
            insert(tail, other_ptr);
            other_ptr = next_other;
        }
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (size() <= 1) return;

        node *current = head;
        while (current != nullptr) {
            node *temp = current->next;
            current->next = current->prev;
            current->prev = temp;
            current = temp;
        }

        // Swap head and tail
        node *temp = head;
        head = tail;
        tail = temp;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (size() <= 1) return;

        iterator it = begin();
        iterator next_it = it;
        ++next_it;

        while (next_it != end()) {
            if (*it == *next_it) {
                next_it = erase(next_it);
            } else {
                ++it;
                ++next_it;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
