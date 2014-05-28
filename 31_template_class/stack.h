#ifndef STACK_H
#define STACK_H

#include <iostream>

template <class T>
class Stack 
{
	public:
		Stack();
            ~Stack();
		void push(T i);
		T pop();
		void print(unsigned int id);
	private:
		int top;
		T st[100];
};

template <class T>
Stack<T>::Stack()
{
      top = -1;
}

template <class T>
Stack<T>::~Stack()
{
      std::cout << "destructor" << std::endl;
}

template <class T>
void Stack<T>::push(T i)
{
	st[++top] = i;
}

template <class T>
T Stack<T>::pop()
{
	return st[top--];
}

template <class T>
void Stack<T>::print(unsigned int id)
{
	std::cout << st[id] << std::endl;
}
#endif
