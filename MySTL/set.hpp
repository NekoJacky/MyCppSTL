//
// Created by Apple Inc on 2023/9/4.
//

#ifndef MYCPPSTL_SET_HPP
#define MYCPPSTL_SET_HPP

#include "rb_tree.hpp"

namespace DemoSTL
{
    template<class Key, class Compare = std::less<Key>, class Alloc = alloc>
    class set
    {
        using key_type = Key;
        using value_type = Key;
        using key_compare = Compare;
        using value_compare = Compare;

    private:
        template<class T>
        struct identity
        {
            const T& operator()(const T& x)const { return x; }
        };
        using rep_type  = rb_tree<key_type, value_type, identity<value_type>, key_compare, alloc>;

        rep_type t;

    public:
        using pointer           = typename rep_type::const_pointer;
        using const_pointer     = typename rep_type::const_pointer;
        using reference         = typename rep_type::const_reference;
        using const_reference   = typename rep_type::const_reference;
        using iterator          = typename rep_type::const_iterator;
        using const_iterator    = typename rep_type::const_iterator;

        using size_type         = typename rep_type::size_type;
        using difference_type   = typename rep_type::difference_type;

    public:
        set(): t(Compare()) {}

        explicit set(const Compare& comp): t(comp) {}

        template<class InputIterator>
        set(InputIterator first, InputIterator last): t(Compare())
        { t.insert_unique(first, last); }

        template<class InputIterator>
        set(InputIterator first, InputIterator last, const Compare& comp): t(comp)
        { t.insert_unique(first, last); }

        set(const set<Key, Compare, Alloc>& x): t(x.t) {}

        set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x)
        {
            t = x.t;
            return *this;
        }

    public:
        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return t.key_comp(); }
        iterator begin() const { return t.begin(); }
        iterator end() const { return t.end(); }
        bool empty() { return t.empty(); }
        size_type size() { return t.size(); }
        size_type max_size() { return t.max_size(); }

        using pair_iterator_bool    = std::pair<iterator, bool>;
        using rep_iterator          = typename rep_type::iterator;

        pair_iterator_bool insert(const value_type& x)
        {
            std::pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
            return pair_iterator_bool(p.first, p.second);
        }
        void clear() { t.clear(); }

        iterator find(const key_type& k) const { return t.find(k); }
    };
}

#endif //MYCPPSTL_SET_HPP
