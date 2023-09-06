//
// Created by Jacky on 2023/7/6.
//

#ifndef MYCPPSTL_STLITERATOR_H
#define MYCPPSTL_STLITERATOR_H

#include <cstddef>

/* 迭代器基类 */

namespace DemoSTL
{
    struct input_iterator_tag {};                                   // 输入迭代器
    struct output_iterator_tag {};                                  // 输出迭代器
    struct forward_iterator_tag: input_iterator_tag {};             // 向前迭代器
    struct bidirectional_iterator_tag: forward_iterator_tag {};     // 双向迭代器
    struct random_access_iterator_tag: bidirectional_iterator_tag{};// 随机访问迭代器

    // 迭代器基类
    template<class Category, class T, class Distance=ptrdiff_t, class Pointer=T*, class Reference=T&>
    struct iterator
    {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
    };

    template<class Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::difference_type      difference_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
    };

    // 针对原生指针特化
    template<class T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    // 针对const指针特化
    template<class T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef const T*                    pointer;
        typedef const T&                    reference;
    };

    template<class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type * distance_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type *> (0);
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type * value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type *> (0);
    }

    // distance
    // 用于计算两个迭代器之间的距离
    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
    {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return distance_(first, last, category());
    }

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance_(InputIterator first, InputIterator last, input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last)
        {
            first++;
            n++;
        }
        return n;
    }

    template<class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    distance_(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
    {
        return last-first;
    }

    // advance
    // 用于将特定的迭代器移动特定的距离
    template<class Iterator, class Distance>
    inline void advance(Iterator &i, Distance n)
    {
        advance_(i, n, iterator_category(i));
    }

    template<class InputIterator, class Distance>
    inline void advance_(InputIterator &i, Distance n, input_iterator_tag)
    {
        while(n--) i++;
    }

    template<class BidirectionalIterator, class Distance>
    inline void advance_(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
    {
        if(n >= 0) while(n--) i++;
        else while(n++) i--;
    }
    template<class RandomAccessIterator, class Distance>
    inline void advance_(RandomAccessIterator &i, Distance n, random_access_iterator_tag)
    {
        i += n;
    }
}

#endif //MYCPPSTL_STLITERATOR_H
