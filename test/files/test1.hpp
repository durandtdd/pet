#ifndef TEST1_HPP
#define TEST1_HPP

#ifdef TEST_DLL_EXPORT
    #define DLL_API __declspec(dllexport)
#else
    #define DLL_API __declspec(dllimport)
#endif

namespace cpp
{
    DLL_API void uselessFunction();
}

DLL_API int add(int a, int b);

DLL_API double add(double a, double b);



class DLL_API Class
{
    public:
        Class();
        ~Class();

        int publicMethod1();
        void publicMethod2();

    protected:
        int protectedMethod1();
        void protectedMethod2();

    private:
        int privateMethod1();
        void privateMethod2();

    private:
        int m_member;
};

#endif
