//
// Created by Jacky on 2023/7/18.
//

#ifndef MYCPPSTL_VECTOR_H
#define MYCPPSTL_VECTOR_H

#include "memory.hpp"

namespace DemoSTL
{
    /* class vector
     * 需要一长串的连续空间
     * 空间只增不减
     * iterator可以直接使用原生指针 */

    template<class T, class alloc=alloc>
    class vector
    {
    public:
        typedef T               value_type;
        typedef value_type*     pointer;
        typedef value_type*     iterator;
        typedef value_type&     reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        using data_alloc = simple_alloc<T>;

    protected:
        typedef simple_alloc<value_type, alloc> data_allocator;
        iterator start;
        iterator finish;
        iterator end_of_storage;

        void insert_aux(iterator position, const T& x)
        {
            if(finish != end_of_storage)
            {
                construct(position, *(position-1));
                finish++;
                std::copy_backward(position, finish-2, finish-1);
                *(position) = x;
            }
            else
            {
                size_type old_size = size();
                size_type len = old_size!=0?2*old_size:1;
                iterator new_start = data_alloc::allocate(len);
                iterator new_finish = new_start;

                try{
                    new_finish = uninitialized_copy(start, position, new_start);
                    construct(new_finish, x);
                    new_finish++;
                    new_finish = uninitialized_copy(position, finish, new_finish);
                }
                catch(...){
                    destroy(new_start, new_finish);
                    data_alloc::deallocate(new_start, len);
                    throw;
                }

                destroy(start, finish);
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = start + len;
            }
        }
        void deallocate()
        {
            if(start) data_alloc::deallocate(start, end_of_storage-start);
        }
        iterator alloc_and_fill(size_type n, const T& x)
        {
            iterator result = data_allocator::allocate(n);
            uninitialized_fill_n(result, n, x);
            return result;
        }
        void fill_initialized(size_type n, const T& x)
        {
            start = alloc_and_fill(n, x);
            finish = start + n;
            end_of_storage = finish;
        }

    public:
        iterator begin() { return start; }
        iterator end() { return finish; }
        [[nodiscard]] size_type size() const { return size_type(finish-start); }
        [[nodiscard]] size_type capacity() const { return size_type(end_of_storage-start); }
        bool empty(){ return start == finish; }
        reference operator[](size_type n) { return *(start+n); }

        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        explicit vector(size_type n) { fill_initialized(n, T()); }
        vector(size_type n, const T& value) { fill_initialized(n, value); }
        vector(int n, const T& value) { fill_initialized(n, value); }
        vector(long n, const T& value) { fill_initialized(n, value); }
        ~vector()
        {
            destroy(start, finish);
            deallocate();
        }

        reference front() { return *start; }
        reference back() { return *(finish-1); }
        void push_back(const T& x)
        {
            if(end_of_storage != finish)
            {
                construct(finish, x);
                finish++;
            }
            else
            {
                insert_aux(finish, x);
            }
        }
        void pop_back()
        {
            finish--;
            destroy(finish);
        }
        iterator erase(iterator position)
        {
            if(position + 1 != finish)
                std::copy(position+1, finish, position);
            finish--;
            destroy(finish);
            return position;
        }
        iterator erase(iterator position_start, iterator position_finish)
        {
            size_type dif = position_finish - position_start;
            iterator finish_tmp = finish;
            if(position_finish + 1 != finish)
                std::copy(position_finish+1, finish, position_start);
            finish -= dif;
            destroy(finish, finish_tmp);
            return position_start;
        }
        void resize(size_type new_size, const T& x)
        {
            if(new_size < size())
                erase(start+new_size, finish);
            else
                insert(finish, new_size-size(), x);
        }
        void resize(size_type new_size) { resize(new_size, T()); }
        void clear() { erase(start, finish); }
    };
} // DemoSTL

#endif //MYCPPSTL_VECTOR_H
