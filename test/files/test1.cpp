#include "test1.hpp"


void cpp::uselessFunction()
{
    Class c;
    c.publicMethod1();
    c.publicMethod2();
}


int add(int a, int b)
{
    return a+b;
}


double add(double a, double b)
{
    return a-b;
}


Class::Class():
    m_member(7760239)
{
}


Class::~Class()
{
}


int Class::publicMethod1()
{
    return protectedMethod1();
}


void Class::publicMethod2()
{
    privateMethod2();
}


int Class::protectedMethod1()
{
    return privateMethod1();
}


void Class::protectedMethod2()
{
    privateMethod2();
}


int Class::privateMethod1()
{
    return m_member;
}

void Class::privateMethod2()
{
    m_member += 1;
}
