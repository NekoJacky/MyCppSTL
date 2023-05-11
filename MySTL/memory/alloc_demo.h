//
// Created by Jacky on 2023/4/18.
//

#ifndef MYCPPSTL_ALLOC_DEMO_H
#define MYCPPSTL_ALLOC_DEMO_H

#ifndef __THROW_BAD_ALLOC
#include<iostream>
#define __THROW_BAD_ALLOC std::cerr << "Out of memory!" << std::endl; exit(1)
#endif

namespace DemoSTL
{
    // malloc-based allocator，第一级配置器
    // 直接使用malloc，free
    template<int inst>
    class _malloc_alloc_template
    {
    private:
        // 以下都是函数指针
        // oom: out of memory
        static void *oom_malloc(size_t);
        static void *oom_realloc(void *, size_t);
        static void (* _malloc_alloc_oom_handler)();

    public:
        static void *allocate(size_t n)
        {
            void *result = malloc(n);
            if(result == nullptr)
                result = oom_malloc(n);
            return result;
        }

        static void deallocate(void *p)
        {
            free(p);
        }

        static void *reallocate(void *p, size_t old_size, size_t new_size)
        {
            void *result = realloc(p, new_size);
            if(result == nullptr)
                result = oom_realloc(p, new_size);
            return result;
        }

        static void (*set_malloc_handler(void (*f) ()))()
        {
            void (*old)() = _malloc_alloc_oom_handler;
            _malloc_alloc_oom_handler = f;
            return old;
        }
    };// class _malloc_alloc_template

    template<int inst>
    void (* _malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = nullptr;

    template<int inst>
    void * _malloc_alloc_template<inst>::oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void *result;

        while(true)
        {
            my_malloc_handler = _malloc_alloc_oom_handler;
            if(my_malloc_handler == nullptr) {__THROW_BAD_ALLOC;}
            (*my_malloc_handler)();
            result = malloc(n);
            if(result) return result;
        }
    }

    template<int inst>
    void * _malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
    {
        void (*my_malloc_handler)();
        void * result;

        while(true)
        {
            my_malloc_handler = _malloc_alloc_oom_handler;
            if(my_malloc_handler == nullptr) {__THROW_BAD_ALLOC;}
            (*my_malloc_handler)();
            result = realloc(p, n);
            if(result) return result;
        }
    }

    typedef _malloc_alloc_template<0> malloc_alloc;

    // malloc_alloc_template end


    // TO DO: default alloc template
} // DemoSTL

#endif //MYCPPSTL_ALLOC_DEMO_H
