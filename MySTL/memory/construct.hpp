//
// Created by Jacky on 2023/4/18.
//

#ifndef MYCPPSTL_CONSTRUCT_HPP
#define MYCPPSTL_CONSTRUCT_HPP

#include <new>
#include <type_traits>

// 定义了构造与析构的方法construct()与destroy()
// 这两个函数都是全局函数

namespace DemoSTL
{
    // 使用了placement new
    template<class T1, class T2>
    inline void construct(T1* p, T2& value)
    {
        new (p) T1(value);
    }

    template<class T>
    inline void construct(T* p)
    {
        new (p) T();
    }

    template<class T>
    inline void destroy(T* p)
    {
        p->~T();
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator First, ForwardIterator Last)
    {
        _destroy(First, Last, value_type(First));
    }

    template<class ForwardIterator, class T>
    inline void _destroy(ForwardIterator First, ForwardIterator Last, T*)
    {
        // 类型萃取 type traits
        typedef typename std::is_trivially_destructible<T> trivial_destroy;
        _destroy_aux(First, Last, trivial_destroy());
    }

    // 如果没有 trivial destructor，则需要调用其析构函数
    template<class ForwardIterator>
    inline void _destroy_aux(ForwardIterator First, ForwardIterator Last, std::false_type)
    {
        while(First < Last)
        {
            destroy(&*First);
            First++;
        }
    }

    // 有 trivial destructor，即使用默认的析构函数，则什么都不需要做。
    template<class ForwardIterator>
    inline void _destroy_aux(ForwardIterator First, ForwardIterator Last, std::true_type) {}

    // 特化
    inline void destroy(char*, char*) {};
    inline void destroy(wchar_t*, wchar_t*) {};
}


#endif //MYCPPSTL_CONSTRUCT_HPP
