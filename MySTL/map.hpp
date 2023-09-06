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

        auto value_compare(const value_type& x, const value_type& y)
        { return key_compare(x.first, y.first); }

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
    };
} // DemoSTL

#endif //MYCPPSTL_MAP_HPP
