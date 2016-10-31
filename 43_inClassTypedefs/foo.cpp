
#include "foo.h"

Foo::Foo()
{
    value_ = 1; 
}

Foo::~Foo()
{
    
}

double Foo::multiply(double _a)
{
    value_ = value_ * _a; 
}

double Foo::getValue()
{
    return value_;
}
