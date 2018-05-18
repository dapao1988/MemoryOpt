//============================================================================
// Name        : MemoryOpt.cpp
// Author      : wenbing.wang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


//平台：Visual Stdio 2008
#include <iostream>
#include <stdio.h>
#include <malloc.h>
//三种形式的new
//void* operator new (std::size_t size) throw (std::bad_alloc);
//void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) throw();
//void* operator new (std::size_t size, void* ptr) throw();
//A* a = new A; //调用throwing(1)
//A* a = new(std::nothrow) A; //调用nothrow(2)


/**
 * 全局operator new function
 */
void* operator new(size_t size)
{
    if (void* mem = malloc(size))
    {
    	 std::cout<<"call global operator new"<<std::endl;
        return mem;
    }
    else
        throw std::bad_alloc();
}

void operator delete(void *mem) noexcept
{
	 std::cout<<"call global operator delete"<<std::endl;
    free(mem);
}

class A
{
public:
    A()
    {
        std::cout<<"call A constructor"<<std::endl;
    }

    ~A()
    {
        std::cout<<"call A destructor"<<std::endl;
    }

    /**
     * 类重载的 operator new function，new时会优先调用类operator new，如果没有再调用全局function
     * 注意，这里的重载遵循作用域覆盖原则，即在里向外寻找operator new的重载时，只要找到operator new()函数就不再向外查找；
     * 如果参数符合则通过，如果参数不符合则报错，而不管全局是否还有相匹配的函数原型。
     * 比如如果这里只将A中operator new(size_t, const std::nothrow_t&)删除掉，
     * 就会报错：error C2660: “A::operator new”: 函数不接受 2 个参数
     */
    void* operator new(size_t size)
    {
        std::cout<<"call A::operator new"<<std::endl;
        return malloc(size);
    }

    void* operator new(size_t size, const std::nothrow_t& nothrow_value)
    {
        std::cout<<"call A::operator new nothrow"<<std::endl;
        return malloc(size);
    }

    void operator delete(void *mem) noexcept
    {
    	 std::cout<<"call A::operator operator delete"<<std::endl;
        free(mem);
    }

};
int main(int argc, char* argv[])
{
    A* p1 = new A;
    delete p1;

    A* p2 = new(std::nothrow) A;
    delete p2;


    return 0;
}
