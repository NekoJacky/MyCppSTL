//
// Created by Apple Inc on 2023/4/18.
//

#ifndef MYCPPSTL_CONSTRUCT_DEMO_H
#define MYCPPSTL_CONSTRUCT_DEMO_H

#include <new>

namespace DemoSTL
{
    template<class T1, class T2>
    inline void construct(T1* p, T2& value)
    {
        new(p) T1(value);
    }

    template<class T>
    inline void destroy(T* p)
    {
        p->~T();
    }
}


#endif //MYCPPSTL_CONSTRUCT_DEMO_H
