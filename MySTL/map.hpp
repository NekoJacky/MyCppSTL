//
// Created by Jacky on 2023/9/6.
//

#ifndef MYCPPSTL_MAP_HPP
#define MYCPPSTL_MAP_HPP

namespace DemoSTL
{
    template<class T1, class T2>
    struct pair
    {
        using first_type    = T1;
        using second_type   = T2;

        T1 first;
        T2 second;

        pair(): first(T1()), second(T2()) {}
        pair(const T1& a, const T2& b): first(a), second(b) {}
    };

    template<class Pair>
    struct select1st
    {
        using arguement_type    = Pair;
        using result_type       = typename Pair::first;

        // const typename Pair::first& operator()(Pair& x) const { return x.first; }
        result_type& operator()(arguement_type& x) const { return x.first; }
    };

    template<class Key, class T, class Compare=std::less<Key>, class Alloc=alloc>
    class map
    {
        using key_type      = Key;
        using data_type     = T;
        using mapped_type   = T;
        using value_type    = pair<const key_type, data_type>;
        using key_compare   = Compare;

        /*auto value_compare(const value_type& x, const value_type& y)
        { return key_compare(x.first, y.first); }*/

        class value_compare
        {
            friend class map<Key, T, Compare, Alloc>;
        protected:
            Compare comp;
            explicit value_compare(Compare c): comp(c) {}
        public:
            bool operator() (const value_type& x, const value_type& y)
            { return key_compare(x.first, y.first); }
        };

    private:
        using rep_type  = rb_tree<Key, T, select1st<value_type>, key_compare, Alloc>;
        rep_type t;

    public:
        using pointer           = typename rep_type::pointer;
        using const_pointer     = typename rep_type::const_pointer;
        using reference         = typename rep_type::reference;
        using const_reference   = typename rep_type::const_reference;
        using iterator          = typename rep_type::iterator;
        using const_iterator    = typename rep_type::const_iterator;
        using size_type         = typename rep_type::size_type;
        using difference_type   = typename rep_type::difference_type;

        map(): t(Compare()) {}
        explicit map(const Compare& comp): t(comp) {}

        template<class InputIterator>
        map(InputIterator first, InputIterator last): t(Compare())
        { t.insert_unique(first, last); }

        template<class InputIterator>
        map(InputIterator first, InputIterator last, const Compare& comp): t(comp)
        { t.insert_unique(first, last); }

    public:
        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return value_compare(t.key_comp()); }
        iterator begin() { return t.begin(); }
        const_iterator begin() const { return t.begin(); }
        iterator end() { return t.end(); }
        const_iterator end() const { return t.end(); }
        bool empty() { return t.empty(); }
        size_type size() { return t.size(); }
        size_type max_size() { return t.max_size(); }

        pair<iterator, bool> insert(const value_type &x)
        { return t.insert_unique(x); }

        template<class InputIterator>
        void insert(InputIterator first, InputIterator last)
        { return t.insert_unique(first, last); }

        T& operator[](const key_type &k)
        {
            return (*((insert(value_type(k, T()))).first)).second;
        }

        void clear() { t.clear(); }

        iterator find(const key_type &k) { return t.find(k); }
        const_iterator find(const key_type& k) const { return t.find(k); }
    };
} // DemoSTL

#endif //MYCPPSTL_MAP_HPP
