//
// Created by Jacky on 2023/7/31.
//

#ifndef MYCPPSTL_STACK_HPP
#define MYCPPSTL_STACK_HPP

/* 本项目并未实现deque，不过实现了list
 * 因此使用list作为底层容器来实现stack */

#include "list.hpp"

/* stack并未提供一种底层容器，而是修改了其它容器接口
 * 因此应当是配接器而非容器
 * stack没有迭代器 */

namespace DemoSTL
{
    template<class T, class Sequence=list<T>>
    class stack
    {
    public:
        using value_type    = typename Sequence::value_type;
        using size_type     = typename Sequence::size_type;
        using reference     = typename Sequence::reference;
        using ptrdiff_t     = typename Sequence::ptrdiff_t;
        using const_reference   = typename Sequence::const_reference;
    protected:
        Sequence s;

    public:
        bool empty() { return s.empty(); }
        size_type size() { return s.size(); }
        reference top() { return s.back(); }
        const_reference top() const { return s.back(); }
        void push(const T& x) { s.push_back(x); }
        void pop() { s.pop_back(); }
        void swap(stack<T, Sequence> &x) { s.swap(x.s); }
    };
} // DemoSTL

#endif //MYCPPSTL_STACK_HPP
