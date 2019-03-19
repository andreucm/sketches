#include <iostream>
#include <list>
#include <memory>

//forward declaration of the class
class Foo;

//class
class Foo
{
    protected:
        std::shared_ptr<int> ptr_;

    public:
        Foo(unsigned int _id) :
            ptr_(new int(_id))
        {
            std::cout << "Constructor with id_: " << *ptr_ << std::endl;
        }

        ~Foo()
		{
			std::cout << "Destructor " << std::endl;
			ptr_.reset();
		}

		//void setPtr(){ptr_.reset(&id_);}
		int getPtrValue() const { return *ptr_;}
};

int main()
{
	Foo my_foo(33);
	//std::cout << "getPtrValue(): " << my_foo.getPtrValue() << std::endl;
    return 0;
}
