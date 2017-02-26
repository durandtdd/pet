#ifndef PET_DEF_HPP
#define PET_DEF_HPP

#include <cstdint>


// Disable warnings
#ifdef _MSC_VER
    #pragma warning(disable: 4251) // Class member not exported (string, vector, map...)
    #pragma warning(disable: 4275) // Class parent not exported (exception)
#endif


// Namespace
#define PET_BEGIN_NAMESPACE namespace pet {
#define PET_END_NAMESPACE }
#define PET_USE_NAMESPACE using namespace pet;


// DLL defines
#ifndef PET_STATIC_LIB
    #ifdef PET_DLL_EXPORT
        #define PET_DLL __declspec(dllexport)
    #else
        #define PET_DLL __declspec(dllimport)
    #endif
#endif



PET_BEGIN_NAMESPACE


// Typedefs for int types
typedef std::uint8_t uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;


PET_END_NAMESPACE


#endif
