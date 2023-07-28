//
// Created by Jacky on 2023/7/26.
//

#ifndef MYCPPSTL_LIST_HPP
#define MYCPPSTL_LIST_HPP

#include "memory.hpp"

/* 环状双向链表 */

namespace DemoSTL
{
    /* list node */
    template<class T>
    struct list_node
    {
        typedef void* pointer_to_node;

        pointer_to_node p_prev;
        pointer_to_node p_next;
        T data;
    };

    /* list iterator */
    template<class T, class Ref, class Ptr>
    struct list_iterator
    {
        typedef list_iterator<T, T&, T*>    iterator;
        typedef list_iterator<T, Ref, Ptr>  self;

        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Ref                         reference;
        typedef Ptr                         pointer;
        typedef list_node<T>*               link_type;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;

        link_type p_node;     // 这是一个指向节点的指针

        list_iterator() = default;
        explicit list_iterator(link_type link): p_node(link) {};
        explicit list_iterator(const iterator& iterator): p_node(iterator.p_node) {};

        bool operator==(const self& x) { return p_node == x.p_node; }
        bool operator!=(const self& x) { return p_node != x.p_node; }
        reference operator*() { return p_node->data; }
        // 重载->运算符的标准做法，->运算符会不停解引用得到的指针，直至得到需要的数据
        pointer operator->() { return &(operator*()); }
        self& operator++()
        {
            p_node = (link_type)p_node->p_next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--()
        {
            p_node = (link_type)p_node->p_prev;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };

    /* class list */
    template<class T, class alloc=alloc>
    class list
    {
    public:
        typedef T                           value_type;
        typedef T&                          reference;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;
        typedef list_iterator<T, T&, T*>    iterator;

    protected:
        typedef list_node<T> list_node;
        typedef simple_alloc<list_node, alloc> list_node_alloc;
    public:
        typedef list_node* link_type;

    protected:
        link_type p_node;                 // 指向头节点&尾端节点

    public:
        iterator begin() { return p_node->p_next; }
        iterator end() { return p_node; }
        bool empty() { return p_node->p_next == p_node; }
        size_type size() { return distance(begin(), end()); }
        reference front() { return *begin(); }
        reference back() { return *(end()--); }
    protected:
        link_type get_node() { return list_node_alloc::allocate(); }
        void put_node(link_type p) { list_node_alloc::deallocate(p); }
        link_type create_node(const T& x)
        {
            link_type p = get_node();
            construct(&p->data, x);
            return p;
        }
        void destroy_node(link_type p)
        {
            destroy(&p->data);
            put_node(p);
        }

        void empty_initialize()
        {
            p_node = get_node();
            p_node->p_next = p_node;
            p_node->p_prev = p_node;
        }
        iterator insert(iterator position, const T& x)
        {
            link_type tmp = create_node(x);
            tmp->p_next = position.p_node;
            tmp->p_prev = position.p_node->p_prev;
            position.p_node->p_prev = tmp;
            link_type (position.p_node->p_prev)->p_next = tmp;
            return tmp;
        }
        iterator erase(iterator position)
        {
            link_type next = position.p_node->p_next;
            link_type prev = position.p_node->p_prev;
            prev->p_next = next;
            next->p_prev = prev;
            destroy_node(position.p_node);
            return next;
        }

    public:
        list() { empty_initialize(); }          // default;

        void push_back(const T& x) { insert(end(), x); }
        void push_front(const T& x) { insert(begin(), x); }
        void pop_back() { erase(--end()); }
        void pop_front() { erase(begin()); }
        void clear();
    };

    template<class T, class alloc>
    void list<T, alloc>::clear()
    {
        link_type cur = p_node->p_next;
        while(cur != p_node)
        {
            link_type tmp = cur;
            cur = tmp->p_next;
            destroy_node(tmp);
        }
        p_node->p_next = p_node;
        p_node->p_prev = p_node;
    }
}

#endif //MYCPPSTL_LIST_HPP
