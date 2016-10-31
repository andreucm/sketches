
#include <iostream>
#include <memory>
#include "foo.h"
 
// class Foo; 

int main()
{
    std::shared_ptr<Foo> foo_ptr_1;
    Foo::Ptr foo_ptr_2;
    return 1; 
}