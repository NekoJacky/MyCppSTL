//
// Created by Apple Inc on 2023/4/18.
//

#ifndef MYCPPSTL_CONSTRUCT_DEMO_H
#define MYCPPSTL_CONSTRUCT_DEMO_H

#include <new>
#include <type_traits>

//定义了构造与析构的方法construct()与destroy()
namespace DemoSTL
{
    template<class T1, class T2>
    inline void construct(T1* p, T2& value)
    {
        new (p) T1(value);
    }
    // 使用了placement new

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
    inline void destory(ForwardIterator First, ForwardIterator Last)
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
}


#endif //MYCPPSTL_CONSTRUCT_DEMO_H
