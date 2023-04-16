//
// Created by Apple Inc on 2023/4/14.
//

#include "MySTL/allocator_demo.h"
#include "MySTL/memory_demo.h"

struct test
{
    DemoSTL::allocator_demo<int> test_allocator_demo;
    test();
};

test::test()
{
    DemoSTL::allocator_demo<int> tmp;
    this->test_allocator_demo = tmp;
}
