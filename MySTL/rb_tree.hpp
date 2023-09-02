//
// Created by Jacky on 2023/8/24.
//

#ifndef MYCPPSTL_RB_TREE_HPP
#define MYCPPSTL_RB_TREE_HPP

#include "memory.hpp"

namespace DemoSTL
{
    typedef bool rb_tree_type;
    const rb_tree_type red = false;
    const rb_tree_type black = true;

    struct rb_tree_node_base
    {
        typedef rb_tree_type color_type;
        typedef rb_tree_node_base* base_ptr;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr min(base_ptr x)
        {
            while(x->left != nullptr)
                x = x->left;
            return x;
        }

        static base_ptr max(base_ptr x)
        {
            while(x->right != nullptr)
                x = x->right;
            return x;
        }
    };

    template<class Value>
    struct rb_tree_node: rb_tree_node_base
    {
        typedef rb_tree_node<Value>* link_type;
        Value value;
    };

    struct rb_tree_iterator_base
    {
        using base_ptr          = rb_tree_node_base::base_ptr;
        using iterator_category = bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        base_ptr p_node;

        void increment()
        {
            /* 有右子节点，向右走，然后沿左子树走到底 */
            if(p_node->right != nullptr)
            {
                p_node = p_node->right;
                while(p_node->left != nullptr)
                    p_node = p_node->left;
            }
            else
            {
                /* 没有右子节点，找父节点
                 * 如果该节点是父节点的右子节点
                 * 一直找父节点直到不为右子节点 */
                base_ptr p = p_node->parent;
                while(p_node == p->right)
                {
                    p_node = p;
                    p = p_node->parent;
                }
                /* 若此时右子节点不为父节点 则父节点为解答
                 * 否则p_node为解答
                 * 这是为了防止出现寻找根节点的下一节点时
                 * 根节点没有右子节点
                 * 此时p_node->right == p == nullptr*/
                if(p_node->right != p)
                    p_node = p;
            }
        }

        void decrement()
        {
            if(p_node->color == red &&
               p_node->parent->parent == p_node)
            {
                /* p_node是header */
                p_node = p_node->right;
            }
            else if(p_node->left != nullptr)
            {
                p_node = p_node->left;
                while(p_node->right != nullptr)
                    p_node = p_node->right;
            }
            else
            {
                base_ptr p = p_node->parent;
                while(p_node == p->left)
                {
                    p_node = p;
                    p = p_node->parent;
                }
                if(p_node->left != p)
                    p_node = p;
            }
        }
    };

    template<class Value, class Ref, class Ptr>
    struct rb_tree_iterator: rb_tree_iterator_base
    {
        using value_type        = Value;
        using reference         = Ref;
        using pointer           = Ptr;
        using iterator          = rb_tree_iterator<Value, Value&, Value*>;
        using const_iterator    = rb_tree_iterator<Value, const Value&, const Value*>;
        using self              = rb_tree_iterator<Value, Ref, Ptr>;
        using link_type         = rb_tree_node<Value>*;

        rb_tree_iterator() = default;
        explicit rb_tree_iterator(link_type x) : rb_tree_iterator_base() { p_node = x; }
        explicit rb_tree_iterator(const iterator& i) : rb_tree_iterator_base() { p_node = i.p_node; }

        reference operator*() {return link_type(p_node)->value;}
        #ifndef NO_ARROW_OPERATOR
        pointer operator->() {return &(operator*());}
        #endif
        self& operator++() {increment(); return *this;}
        self operator++(int)
        {
            self tmp = *this;
            increment();
            return tmp;
        }
        self& operator--() {decrement(); return *this;}
        self operator--(int)
        {
            self tmp = *this;
            decrement();
            return tmp;
        }
    };

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc=alloc>
    class rb_tree
    {
    protected:
        using void_ptr                  = void*;
        using base_ptr                  = rb_tree_node_base*;
        using rb_tree_node              = rb_tree_node<Value>;
        using rb_tree_node_allocator    = simple_alloc<rb_tree_node, alloc>;
        using color_type                = typename rb_tree_node::color_type;
    public:
        using key_type                  = Key;
        using value_type                = Value;
        using pointer                   = value_type*;
        using const_pointer             = const value_type*;
        using reference                 = value_type&;
        using const_reference           = const value_type&;
        using link_type                 = rb_tree_node*;
        using size_type                 = size_t;
        using difference_type           = ptrdiff_t;
    protected:
        link_type get_node() { return rb_tree_node_allocator::allocate(); }
        void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

        link_type create_node(const_reference x)
        {
            link_type tmp = get_node();
            try
            {
                construct(&tmp->value, x);
            }
            catch(...)
            {
                put_node(tmp);
            }
            return tmp;
        }

        link_type clone_node(link_type x)
        {
            link_type tmp = create_node(x->value);
            tmp->color = x->color;
            tmp->right = nullptr;
            tmp->left = nullptr;
            tmp->parent = nullptr;
            return tmp;
        }

        void destroy_node(link_type p)
        {
            destroy(&p->value);
            put_node(p);
        }

    protected:
        size_type node_count;   // 用于记录节点个数
        link_type header;
        Compare key_compare;    // functional obj，用于比较node值

        // header->parent指向根节点
        link_type root() { return header->parent; }
        link_type left_most() { return header->left; }
        link_type right_most() { return header->right; }

        static link_type &left(link_type x) { return x->left; }
        static link_type &right(link_type x) { return x->right; }
        static link_type &parent(link_type x) { return x->parent; }
        static reference value(link_type x) { return x->value; }
        static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
        static color_type &color(link_type x) { return x->color; }

        static link_type &left(base_ptr x) { return x->left; }
        static link_type &right(base_ptr x) { return x->right; }
        static link_type &parent(base_ptr x) { return x->parent; }
        static reference value(base_ptr x) { return ((link_type)x)->value; }
        static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
        static color_type &color(base_ptr x) { return ((link_type)x)->color; }

        static link_type min(link_type x) { return rb_tree_node_base::min(x); }
        static link_type max(link_type x) { return rb_tree_node_base::max(x); }

    public:
        using iterator  = rb_tree_iterator<value_type , reference, pointer>;

    protected:
        inline void rb_tree_re_balance(rb_tree_node_base* x, rb_tree_node_base*& root)
        {
            // todo rb_tree_re_balance() -4
        }

    private:
        iterator insert_(base_ptr x_, base_ptr y_, const_reference v)
        {
            link_type x = x_;
            link_type y = y_;
            link_type z;
            if(y == header || key_compare(KeyOfValue()(v), key(y)) || x != nullptr)
            {
                z = create_node(v);
                left(y) = z;
                if(y == header)
                {
                    root() = z;
                    right_most() = z;
                }
                else if(y == left_most())
                    left_most() = z;
            }
            else
            {
                z = create_node(v);
                right(y) = z;
                if(y == right_most())
                    right_most() = z;
            }
            parent(z) = y;
            left(z) = nullptr;
            right(z) = nullptr;

            rb_tree_re_balance(z, header->parent);
            node_count++;
            return iterator(z);
        }
        /*link_type copy_(base_ptr x, link_type p)
        {
            return nullptr;
        }*/
        /*void erase_(link_type x)
        {
        }*/
        void init()
        {
            header = get_node();
            color(header) = red;

            root() = nullptr;
            left_most() = header;
            right_most() = header;
        }

    public:
        explicit rb_tree(const Compare& comp = Compare()): node_count(0), key_compare(comp) { init(); }
        /*~rb_tree() { clear(); put_node(header); } todo: clear() -u */

        /*rb_tree<Key, Value, KeyOfValue, Compare>&
        operator=(const rb_tree<Key, Value, KeyOfValue, Compare>& x)
        {
        }*/

    public:
        Compare key_comp() const { return key_compare; }
        iterator begin() { return left_most(); }
        iterator end() { return header; }
        bool empty() { return node_count == 0; }
        size_type size() { return node_count; }
        [[nodiscard]] size_type max_size() const { return size_type(-1); }

    public:
        std::pair<iterator, bool> insert_unique(const_reference v)
        {
            link_type y = header;
            link_type x = root();
            bool comp = true;
            while(x != nullptr)
            {
                y = x;
                comp = key_compare(KeyOfValue()(v), key(x));
                x = comp ? left(x) : right(x);
            }

            iterator j = iterator(y);
            if(comp)
            {
                if(j == begin())
                    return std::pair<iterator, bool>(insert_(x, y, v), true);
                else
                    j--;
            }
            if(key_compare(key(j.p_node), KeyOfValue()(v)))
                return std::pair<iterator, bool>(insert_(x, y, v), true);

            return std::pair<iterator, bool>(j, false);
        }
        iterator insert_equal(const_reference v)
        {
            link_type y = header;
            link_type x = root();
            while(x != nullptr)
            {
                y = x;
                x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
            }
            return insert_(x, y, v);
        }
    };
} // DemoSTL

#endif //MYCPPSTL_RB_TREE_HPP
