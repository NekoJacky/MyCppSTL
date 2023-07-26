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
        pointer_to_node prev;
        pointer_to_node next;
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

        link_type node;     // 这是一个指向节点的指针

        list_iterator() = default;
        explicit list_iterator(link_type link): node(link) {};
        explicit list_iterator(const iterator& iterator): node(iterator.node) {};

        bool operator==(const self& x) { return node == x.node; }
        bool operator!=(const self& x) { return node != x.node; }
        reference operator*() { return node->data; }
        // 重载->运算符的标准做法，->运算符会不停解引用得到的指针，直至得到需要的数据
        pointer operator->() { return &(operator*()); }
        self& operator++()
        {
            node = (link_type)node->next;
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
            node = (link_type)node->prev;
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
    protected:
        typedef list_node<T> list_node;
    public:
        typedef list_node* link_type;
    protected:
        link_type node;
    };
}

#endif //MYCPPSTL_LIST_HPP
