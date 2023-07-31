//
// Created by Jacky on 2023/4/11.
//

#ifndef MYCPPSTL_ALLOCATOR_HPP
#define MYCPPSTL_ALLOCATOR_HPP

#include <iostream>
#include <cstddef>
#include <new>
#include <climits>

namespace DemoSTL
{
    // 一个简单的空间配置器，包装了new和delete
    // 本项目中并未使用该allocator，而是使用了"memory.hpp"及./memory下的allocator

    template<class T>
    inline T* _allocate(ptrdiff_t size, T*)
    {
        std::set_new_handler(nullptr);
        // 当内存不够申请空间失败时，默认抛出std::bad_alloc异常。当使用set_new_handler(new_handler)时，
        // 程序会转向new_handler指向的（我们自己写的）内存处理程序。
        // 当handler为nullptr时，强制令C++认为我们没有自定义的异常处理程序，
        // 即强制C++抛出std::bad_alloc异常。
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
    class allocator
    {
    public:
        typedef T           type_value;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        // rebind机制用来通过一个已经特化的模板类型得到其它的特化类型
        template<class U>
        struct rebind
        {
            typedef allocator<U> other;
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

        [[nodiscard]] size_type max_size() const
        {
            return size_type(UINT_MAX / sizeof(T));
        }
    };

} // DemoSTL

#endif //MYCPPSTL_ALLOCATOR_HPP
