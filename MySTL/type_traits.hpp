//
// Created by Jacky on 2023/7/9.
//

#ifndef MYCPPSTL_TYPE_TRAITS_HPP
#define MYCPPSTL_TYPE_TRAITS_HPP

namespace DemoSTL
{
    struct true_type {};
    struct false_type {};

    struct type_traits
    {
        typedef true_type  first_member;
        typedef false_type has_trivial_default_constructor; // 是否有默认的构造函数
        typedef false_type has_trivial_copy_constructor;    // 是否有复制构造
        typedef false_type has_trivial_assignment_operator; // 是否有自动无效的赋值操作
        typedef false_type has_trivial_destructor;          // 是否有自动无效的析构函数
        typedef false_type is_pod_type;                     // 是否是pod类型的
    };

    /* 其它的实在太多太杂了
     * 直接用std中的<type_traits>吧（笑） */
}

#endif //MYCPPSTL_TYPE_TRAITS_HPP
