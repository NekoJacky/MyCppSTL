//
// Created by Jacky on 2023/4/11.
//

#ifndef MYCPPSTL_ALLOCATOR_DEMO_H
#define MYCPPSTL_ALLOCATOR_DEMO_H

#include <iostream>
#include <cstddef>
#include <new>

namespace DemoSTL
{
    template<class T>
    inline T* _allocate(ptrdiff_t size, T*)
    {
        std::set_new_handler(nullptr);      // 保证分配失败（内存不足）时会抛出异常
        T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
        if(!tmp)
        {
            std::cout << "out of memory" << std::endl;
            exit(1);
        }
        return tmp;
    }

    template<class T>
    inline void _deallocate(T* buffer)
    {
        ::operator delete(buffer);
    }

    template<class T1, class T2>
    inline void _construct(T1* p, const T2& value)
    {
        new(p) T1(value);
    }

    template<class T>
    inline void _destroy(T* ptr)
    {
        ptr -> ~T();
    }

    template<class T>
    class allocator_demo
    {
    public:
        typedef T           type_value;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        template<class U>
        struct rebind
        {
            typedef allocator_demo<U> other;
        };

        pointer allocate(size_type n, const void* hint= nullptr)
        {
            return _allocate((difference_type) n, (pointer) 0);
        }

        void deallocate(pointer p, size_type n)
        {
            _deallocate(p);
        }

        void construct(pointer p, const T& value)
        {
            _construct(p, value);
        }

        void destroy(pointer p)
        {
            _destroy(p);
        }

        const_pointer const_address(const_reference x)
        {
            return (const_pointer)& x;
        }

        size_type max_size() const
        {
            return size_type(INT_MAX / sizeof(T));
        }
    };

} // DemoSTL

#endif //MYCPPSTL_ALLOCATOR_DEMO_H
