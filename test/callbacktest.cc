#include <iostream>
#include "base/base.h"

class calltest
{
public:
    void callb0()
    {
        std::cout << "Method, no arg, no return\n";
    }

    unsigned int callb0ret()
    {
        std::cout << "Method, no arg, returning " << 5 << "\n";
        return 5;
    }

    void callb1(char c)
    {
        std::cout << "Method, arg: " << c << " no return\n";
    }

  void callb2 (char c, char d)
  {
    std::cout << "Method, args: " << c  << " " << d << " no return\n";
  }

    unsigned int callb1ret(char c)
    {
        std::cout << "Method, arg: " << c << " returning " << (unsigned int) c << "\n";
        return (unsigned int) c;
    }
};

void callb0()
{
    std::cout << "Function, no arg, no return\n";
}

unsigned int callb0ret()
{
    std::cout << "Function, no arg, returning " << 10 << "\n";
    return 10;
}

void callb1(char c)
{
    std::cout << "Function, arg: " << c << " no return\n";
}


void callb2(char c, char d)
{
  std::cout << "Function, args: " << c << " " << d << " no return\n";
}


unsigned int callb1ret(char c)
{
    std::cout << "Function, arg: " << c << " returning " << (unsigned int) c << "\n";
    return (unsigned int) c;
}

int main(int argc, char * argv[])
{
    calltest ctest;

    base::functor_0 * f0 = base::make_functor(ctest, &calltest::callb0);
    (*f0)();
    delete f0;
    f0 = base::make_functor(&callb0);
    (*f0)();
    delete f0;

    base::functor_0ret<unsigned int> * f0ret = base::make_functor_ret(ctest, &calltest::callb0ret);
    std::cout << "Got: " << (*f0ret)() << std::endl;
    delete f0;
    f0ret = base::make_functor_ret(&callb0ret);
    std::cout << "Got: " << (*f0ret)() << std::endl;
    delete f0;    

    base::functor_1<char> * f1 = base::make_functor(ctest, &calltest::callb1);
    (*f1)('a');
    delete f1;
    f1 = base::make_functor(&callb1);
    (*f1)('b');
    delete f1;

    base::functor_1ret<char, unsigned int> * f1ret = base::make_functor_ret(ctest, &calltest::callb1ret);
    std::cout << "Got: " << (*f1ret)('a') << std::endl;
    delete f1ret;
    f1ret = base::make_functor_ret(&callb1ret);
    std::cout << "Got: " << (*f1ret)('b') << std::endl;
    delete f1ret;
    
    base::functor_2<char, char> * f2 = base::make_functor(ctest, &calltest::callb2);
    (*f2)('a','x');
    delete f2;
    f2 = base::make_functor(&callb2);
    (*f2)('b','y');
    delete f2;


    
    return 0;
}
