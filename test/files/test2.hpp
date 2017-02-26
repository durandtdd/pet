#ifndef TEST2_HPP
#define TEST2_HPP

#ifdef TEST_DLL_EXPORT
    #define DLL_API __declspec(dllexport)
#else
    #define DLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLL_API void uselessFunction();

DLL_API int add(int a, int b);

DLL_API double add_double(double a, double b);

struct DLL_API Struct
{
    int a;
    double b;
    char* c;
};

#ifdef __cplusplus
}
#endif

#endif
