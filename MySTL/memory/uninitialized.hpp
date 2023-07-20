//
// Created by Jacky on 2023/6/25.
//

#ifndef MYCPPSTL_UNINITIALIZED_HPP
#define MYCPPSTL_UNINITIALIZED_HPP

// 定义了三个全局函数
// 三个函数都是：
//          要么构造全部元素，要么不构造任何元素
//          当任意元素构造失败时，必须将已产生的元素析构掉
// 该文件包含于<memory>

#include "construct.hpp"
#include <algorithm>

namespace DemoSTL
{
    // 是pod类型，则保证效率
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator
    _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type)
    {
        return std::copy(first, last, result);
    }

    // 不是pod类型，则保证安全
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator
    _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type)
    {
        ForwardIterator current = result;
        for(; first != last; first++, current++)
        {
            construct(&*current, *first);
        }
        return current;
    }

    template<class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
    {
        typedef typename std::is_pod<T> is_pod;
        return _uninitialized_copy_aux(first, last, result, is_pod());
    }

    // 将First到Last范围内的对象复制构造于result
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {
        return _uninitialized_copy(first, last, result, value_type(result));
    }

    // 特化
    inline const char * uninitialized_copy(const char* first, const char* last, const char* result)
    {
        memmove((void*)result, first, last-first);
        return result + (last - first);
    }

    // 特化
    inline const wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, const wchar_t* result)
    {
        memmove((void*)result, first, (last-first)*sizeof(wchar_t));
        return result + (last - first);
    }

    // 将x复制多份于First到Last范围内
    template<class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
    {
        _uninitialized_fill(first, last, x, value_type(first));
    }

    template<class ForwardIterator, class T, class T1>
    inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
    {
        typedef typename std::is_pod<T1> is_pod;
        _uninitialized_fill_aux(first, last, x, is_pod());
    }

    template<class ForwardIterator, class T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::true_type)
    {
        std::fill(first, last, x);
    }

    template<class ForwardIterator, class T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::false_type)
    {
        ForwardIterator current = first;
        for(; current != last; current++)
        {
            construct(&*current, x);
        }
    }

    // 将x复制n份于First后n个空间中
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
    {
        return _uninitialized_fill_n(first, n, x, value_type(first));
    }

    template<class ForwardIterator, class Size, class T, class T1>
    inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, T& x, T1*)
    {
        // is_pod用来萃取pod(plain old data，纯旧数据)类型
        typedef typename std::is_pod<T1> is_pod;
        return _uninitialized_fill_n_aux(first, n, x, is_pod());
    }

    // 是pod类型，则保证效率
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, T& x, std::true_type)
    {
        return std::fill_n(first, n, x);
    }

    // 不是pod类型，则保证安全
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, T& x, std::false_type)
    {
        ForwardIterator current = first;
        for(; n > 0; n--, current++)
        {
            construct(&*current, x);
        }
        return current;
    }
} // DemoSTL

#endif //MYCPPSTL_UNINITIALIZED_HPP
