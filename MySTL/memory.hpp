//
// Created by Jacky on 2023/4/16.
//

#ifndef MYCPPSTL_MEMORY_HPP
#define MYCPPSTL_MEMORY_HPP

#include "memory/alloc.hpp"
#include "memory/uninitialized.hpp"
#include "STLIterator.hpp"


#ifdef __USE_MALLOC__
typedef DemoSTL::malloc_alloc alloc;
#else
#define NODE_ALLOCATOR_THREADS
typedef DemoSTL::default_alloc alloc;
#endif


/*
 * memory中包含：
 *      construct.h：    负责对象构造与析构
 *      alloc.h：        内存配置器
 *      uninitialized：  包含一些全局函数，用来填充或复制大块的内存数据
 */


namespace DemoSTL
{
    // simple_alloc是一个配置器接口的包装
    // 使配置器拥有符合STL规范的接口
    template<class T, class alloc=alloc>
    class simple_alloc
    {
    public:
        static T *allocate(size_t n)
        {
            return 0==n ? nullptr : (T*) alloc::allocate(n*sizeof(T));
        }

        static T *allocate()
        {
            return (T*) alloc::allocate(sizeof(T));
        }

        static void deallocate(T *p, size_t n)
        {
            if(n != 0)
            {
                alloc::deallocate(p, n*sizeof(T));
            }
        }

        static void deallocate(T *p)
        {
            alloc::deallocate(p, sizeof(T));
        }
    };
}

#endif //MYCPPSTL_MEMORY_HPP
