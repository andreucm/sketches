#include <iostream>
#include <list>
#include <memory>

//forward declaration of the class
class Foo;

//class 
class Foo
{
    protected:
        unsigned int id_; 
        std::shared_ptr<Foo> next_ptr_; 
            
    public:
        //constructor
        Foo(unsigned int _id) :
            id_(_id),
            next_ptr_(nullptr)
        {
            //
        }
        
        //destructor
        ~Foo(){}
        
        //returns id_
        unsigned int getId() const
        {
            return id_; 
        }
        
        //set next_ptr_ pointer
        void setNextPtr(std::shared_ptr<Foo> & _next_ptr)
        {
            next_ptr_.reset(_next_ptr.get()); 
        }
        
        void resetNextPtr()
        {
            next_ptr_.reset(); 
        }
        
        //print 
        void print() const
        {
            std::cout << "id_: " << id_ << std::endl; 
            if (next_ptr_ != nullptr) std::cout << "next id_: " << next_ptr_->getId() << std::endl; 
            else std::cout << "next id_: nullptr" << std::endl; 
            std::cout << "use_count: " << next_ptr_.use_count() << std::endl; 
        }
};

int main()
{
    //creates a list of shared pointers to Foo objects
    std::list<std::shared_ptr<Foo> > foo_list; 
    for (unsigned int ii = 0; ii<10; ii++)
    {
        //foo_list.push_back( std::make_shared<Foo>(ii) );
        foo_list.push_back( std::shared_ptr<Foo>(new Foo(ii)) );
    }
    
    //print list
    for (auto it = foo_list.begin(); it != foo_list.end(); it++)
    {
        it->get()->print();
        std::cout << "use_count: " << it->use_count() << std::endl << std::endl; 
    }    
    
    //connect each element of the list to the next one
    std::list<std::shared_ptr<Foo> >::iterator it_limit = foo_list.end(); 
    it_limit --; 
    std::list<std::shared_ptr<Foo> >::iterator it_next = foo_list.begin(); 
    it_next++; 
    for (std::list<std::shared_ptr<Foo> >::iterator it = foo_list.begin(); it != it_limit; it++, it_next++)
    {
        it->get()->setNextPtr(*it_next);
    }
    it_next = foo_list.begin(); 
    it_limit->get()->setNextPtr(*it_next);

    //print list
    for (auto it = foo_list.begin(); it != foo_list.end(); it++)
    {
        it->get()->print();
        std::cout << "use_count: " << it->use_count() << std::endl << std::endl; 
    }

//     //debugging message
//     std::cout << __LINE__ << std::endl;
//     
//     //reset next ptr's
//     for (auto it = foo_list.begin(); it != foo_list.end(); it++)
//     {
//         it->get()->resetNextPtr();
//     }
// 
//     //debugging message
//     std::cout << __LINE__ << std::endl;
//    
//     //print list
//     for (auto it = foo_list.begin(); it != foo_list.end(); it++)
//     {
//         it->get()->print();
//     }    
    
    //delete list
    foo_list.clear(); 
    
    //bye bye message
    std::cout << "Bye Bye shared pointers test!" << std::endl;

    //return value
    return 0;
}
