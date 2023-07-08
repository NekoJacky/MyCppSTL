//
// Created by Jacky on 2023/4/14.
//

/* 使用了大量的.hpp文件，原因：
 * 类模板的声明和定义要写在一个.hpp文件中，声明和定义不可以分开
 * 因为分文件的话，main.cpp中只引入.h，在类模板进行二次编译的时候，无法找到类的定义，会出错
 * 所以要将其声明和定义放到一个头文件中。类模板必须要写在.hpp文件中。*/

/* CMake使用：
 * .hpp文件无法使用add_library()命令
 * 直接在上一级目录下的CMakeList.txt使用include_directories()包含就好了
 * 可以使用file(GLOB ...)命令包含 */


#include "MySTL/allocator.hpp"
#include "MySTL/memory.hpp"

struct test
{
    DemoSTL::allocator<int> test_allocator_demo;
    test();
};

test::test()
{
    DemoSTL::allocator<int> tmp;
    this->test_allocator_demo = tmp;
}