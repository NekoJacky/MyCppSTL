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
        explicit rb_tree_iterator(link_type x) : rb_tree_iterator_base() {p_node = x;}
        explicit rb_tree_iterator(const iterator& i) : rb_tree_iterator_base() {p_node = i.p_node;}

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
} // DemoSTL

#endif //MYCPPSTL_RB_TREE_HPP
