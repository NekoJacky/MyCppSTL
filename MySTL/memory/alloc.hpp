//
// Created by Jacky on 2023/4/18.
//

#ifndef MYCPPSTL_ALLOC_HPP
#define MYCPPSTL_ALLOC_HPP

#ifndef THROW_BAD_ALLOC
#include<iostream>
#define THROW_BAD_ALLOC std::cerr << "Out of memory!" << std::endl; exit(1)
#endif

namespace DemoSTL
{
    // malloc-based allocator，第一级配置器
    // 直接使用malloc，free
    template<int inst>
    class malloc_alloc_template
    {
    private:
        // 以下都是函数指针
        // oom: out of memory
        static void *oom_malloc(size_t n);
        static void *oom_realloc(void *p, size_t n);
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
    void (* malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = nullptr;

    template<int inst>
    void * malloc_alloc_template<inst>::oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void *result;

        while(true)
        {
            my_malloc_handler = _malloc_alloc_oom_handler;
            if(my_malloc_handler == nullptr) {THROW_BAD_ALLOC;}
            (*my_malloc_handler)();
            result = malloc(n);
            if(result) return result;
        }
    }

    template<int inst>
    void * malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
    {
        void (*my_malloc_handler)();
        void * result;

        while(true)
        {
            my_malloc_handler = _malloc_alloc_oom_handler;
            if(my_malloc_handler == nullptr) {THROW_BAD_ALLOC;}
            (*my_malloc_handler)();
            result = realloc(p, n);
            if(result) return result;
        }
    }

    typedef malloc_alloc_template<0> malloc_alloc;

    // malloc_alloc_template end


    // default alloc template，第二级配置器
    // 该配置器相当于一个内存池，单次申请大块内存
    // 每次需要小块内存，就由内存池负责分配，回收时也回收到内存池中
    template<bool threads, int inst>
    class default_alloc_template
    {
    public:
        // 这里有点像内存对齐？
        enum{ALIGN = 8};                        // 小型区块上调边界
        enum{MAX_BYTES = 128};                  // 小型区块上限
        enum{FREELISTS = MAX_BYTES / ALIGN};    // free list个数
    private:
        union obj
        {
            obj* free_list_link;
            char client_data[1];    // 作为数据首地址
        };

        // 直接位运算，很巧妙，将不足8的倍数的bytes上调至8的倍数
        static size_t round_up(size_t bytes)
        {
            return (((bytes)+ALIGN-1)&~(ALIGN-1));
        }
    private:
        // volatile修饰的内容随时可能发生变化，每次都必须从内存中再次读取。
        // volatile关键字表示编译器不要对它做过多的优化
        static obj* volatile free_list[FREELISTS];
        // 决定使用第几号free list
        static size_t free_list_index(size_t bytes)
        {
            return (((bytes)+ALIGN-1)/ALIGN-1);
        }
        // 返回大小为n的对象，当没有可用区块时使用
        static void* refill(size_t n);
        // 配置一大块空间内存池
        static char* chunk_alloc(size_t size, int &objs);

        static char* start_free;
        static char* end_free;
        static size_t heap_size;

    public:
        static void* allocate(size_t n);
        static void deallocate(void* p, size_t n);
        static void* reallocate(void* p, size_t old_size, size_t new_size);
    };

    template<bool threads, int inst>
    char* default_alloc_template<threads, inst>::start_free = nullptr;
    template<bool threads, int inst>
    char* default_alloc_template<threads, inst>::end_free = nullptr;
    template<bool threads, int inst>
    size_t default_alloc_template<threads, inst>::heap_size = 0;

    // typename用来指使编译器：obj是一个类型
    template<bool threads, int inst>
    typename default_alloc_template<threads, inst>::obj* volatile
    default_alloc_template<threads, inst>::free_list[FREELISTS] {};

    // 内存池
    template<bool threads, int inst>
    char *default_alloc_template<threads, inst>::chunk_alloc(const size_t size, int &objs)
    {
        char *result;
        size_t need = size * objs;
        size_t left = end_free - start_free;

        if(left > need)
        {
            result = start_free;
            start_free += need;
            return result;
        }
        else if(left > size)
        {
            objs = (int)(left / size);
            need = size * objs;
            result = start_free;
            start_free += need;
            return result;
        }
        else
        {
            // ?
            size_t get = 2*need + round_up(heap_size>>4);

            // 利用内存池中剩余的内存
            if(left > 0)
            {
                obj* volatile *p_free_list = free_list+free_list_index(left);
                ((obj*)start_free)->free_list_link = *p_free_list;
                *p_free_list = (obj*)start_free;
            }

            // 补充内存池
            start_free = (char*)malloc(get);

            // 仍然无法获得内存
            if(start_free == nullptr)
            {
                int i;
                obj * volatile *p_free_list, *p;
                // 尝试从free list中的其它节点获得内存
                for(i = (int)size; i < MAX_BYTES; i += ALIGN)
                {
                    p_free_list = free_list + free_list_index(i);
                    p = *p_free_list;
                    if(p != nullptr)
                    {
                        *p_free_list = p->free_list_link;
                        start_free = (char*)p;
                        end_free = start_free + i;
                        return chunk_alloc(size, objs);
                    }
                }

                // 完全无法获得内存，尝试第一级配置器下的out of memory机制
                // 有可能会导致抛出异常
                end_free = nullptr;
                start_free = (char *)malloc_alloc::allocate(get);
            }

            heap_size += get;
            end_free = start_free += get;
            return chunk_alloc(size, objs);
        }
    }

    template<bool threads, int inst>
    void *default_alloc_template<threads, inst>::refill(size_t n)
    {
        int objs = 20;
        char *chunk = chunk_alloc(n, objs);
        obj *volatile * p_free_list;
        obj *result;
        obj *current_obj, *next_obj;
        int i;

        if(objs == 1) return chunk;
        p_free_list = free_list+free_list_index(n);

        result = (obj*)chunk;
        next_obj = (obj*)(chunk+n);
        *p_free_list = next_obj;

        for(i = 1; ; i++)
        {
            current_obj = next_obj;
            next_obj = next_obj+n;
            if(i == objs-1)
            {
                current_obj->free_list_link = nullptr;
                break;
            }
            else
            {
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }

    template<bool threads, int inst>
    void *default_alloc_template<threads, inst>::reallocate(void *p, size_t old_size, size_t new_size)
    {
        deallocate(p, old_size);
        return allocate(new_size);
    }

    template<bool threads, int inst>
    void default_alloc_template<threads, inst>::deallocate(void *p, size_t n)
    {
        obj* p_obj = (obj*)p;
        obj* volatile * p_free_list;

        if(n > (size_t)MAX_BYTES)
        {
            malloc_alloc::deallocate(p);
        }
        else
        {
            p_free_list = free_list+free_list_index(n);
            p_obj->free_list_link = *p_free_list;
            *p_free_list = p_obj;
        }
    }

    template<bool threads, int inst>
    void *default_alloc_template<threads, inst>::allocate(size_t n)
    {
        obj* volatile * p_free_list;
        obj* result;
        // 大于128bytes
        if(n > (size_t) MAX_BYTES) return malloc_alloc::allocate(n);

        // 小于128bytes
        // 寻找合适的free list
        p_free_list = free_list+free_list_index(n);
        result = *p_free_list;
        if(result == nullptr)
        {
            // free list不足，重新填充free list
            void* r = refill(round_up(n));
            return r;
        }
        // ?
        *p_free_list = result->free_list_link;
        return result;
    }

    typedef default_alloc_template<false, 0> default_alloc;
} // DemoSTL

#endif //MYCPPSTL_ALLOC_HPP
