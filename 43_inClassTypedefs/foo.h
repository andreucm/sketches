#ifndef PROBLEM_H_
#define PROBLEM_H_

//std
#include <memory>

class Foo
{
    protected: 
        double value_; 
        
    public:
        typedef std::shared_ptr<Foo> Ptr;
    
    public: 
        Foo(); 
        virtual ~Foo(); 
        double multiply(double _a); 
        double getValue(); 
};

#endif
