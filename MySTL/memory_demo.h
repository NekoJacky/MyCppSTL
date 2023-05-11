//
// Created by Jacky on 2023/4/16.
//

#ifndef MYCPPSTL_MEMORY_DEMO_H
#define MYCPPSTL_MEMORY_DEMO_H

#include "memory/construct_demo.h"
#include "memory/alloc_demo.h"


// TO DO：写完alloc以后恢复为非注释
/*
#ifdef __USE_MALLOC__
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;
#endif
*/

/*
 * memory中包含：
 *      construct.h：    负责对象构造与析构
 *      alloc.h：        内存配置器
 *      uninitialized：  包含一些全局函数，用来填充或复制大块的内存数据
 */

/*
namespace DemoSTL
{

    // simple_alloc是一个包装，使配置器拥有符合STL规范的接口
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

        static T *deallocate(T *p, size_t n)
        {
            if(n != 0)
            {
                alloc::deallocate(p, n*sizeof(T));
            }
        }

        static T *deallocate(T *p)
        {
            alloc::deallocate(p, sizeof(T));
        }
    };
}
*/

#endif //MYCPPSTL_MEMORY_DEMO_H
