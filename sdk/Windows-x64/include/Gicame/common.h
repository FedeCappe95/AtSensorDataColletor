#ifndef __GICAME__COMMON_H__
#define __GICAME__COMMON_H__


/*
 * Notes on compilers
 *
 * Macro defined only by GCC:
 *   __GNUC__
 *   __GNUC_MINOR__
 *   __GNUC_PATCHLEVEL__
 *   __GNUG__  -->  Testing it is equivalent to testing (__GNUC__ && __cplusplus)
 * Macro defined only by clang:
 *   __clang__
 *   __clang_major__
 *   __clang_minor__
 *   __clang_patchlevel__
 *
 * Branch on compiler:
 *   #if defined(__clang__)
 *   ....
 *   #elif defined(__GNUC__) || defined(__GNUG__)
 *   ....
 *   #elif defined(_MSC_VER)
 *   ....
 *
 * Detect Windows:
 *   #ifdef _WIN32
 *   ...
 *   #endif
*/


// Includes
#include <stdint.h>
#include <stdexcept>
#include <limits.h>
#include <string>


// Type definitions
typedef uint8_t byte_t;
typedef void* ptr_t;


// Macros
#if defined(_WIN32) && !defined(WINDOWS)
    #define WINDOWS
#endif

#if defined(_MSC_VER) && !defined(MSVC)
    #define MSVC
#endif

#ifdef __MINGW32__
    #define MINGW
#endif

#if defined(_MSC_VER)
	#define likely(A) (A)
	#define unlikely(A) (A)
#else
	#define likely(A) __builtin_expect((A) != 0, 1)
	#define unlikely(A) __builtin_expect(bool(A), 0)
#endif

#ifndef GICAME_BUILD_STATIC

    #if defined(_MSC_VER) && defined(GICAME_EXPORTS)
    #define GICAME_API __declspec(dllexport)
    #elif defined(_MSC_VER) && !defined(GICAME_EXPORTS)
    #define GICAME_API __declspec(dllimport)
    #else
    #define GICAME_API
    #endif

    #if defined(_MSC_VER) && defined(GICAME_CRYPTO_EXPORTS)
    #define GICAME_CRYPTO_API __declspec(dllexport)
    #elif defined(_MSC_VER) && !defined(GICAME_CRYPTO_EXPORTS)
    #define GICAME_CRYPTO_API __declspec(dllimport)
    #else
    #define GICAME_CRYPTO_API
    #endif

    #if defined(_MSC_VER) && defined(GICAME_COMMON_EXPORTS)
    #define GICAME_COMMON_API __declspec(dllexport)
    #elif defined(_MSC_VER) && !defined(GICAME_COMMON_EXPORTS)
    #define GICAME_COMMON_API __declspec(dllimport)
    #else
    #define GICAME_COMMON_API
    #endif

#else

    #define GICAME_COMMON_API
    #define GICAME_CRYPTO_API
    #define GICAME_API

#endif

#ifdef MSVC
#define GICAME_PACK_BEGIN __pragma( pack(push, 1) )
#define GICAME_PACK_END ; __pragma( pack(pop) )
#else
#define GICAME_PACK_BEGIN
#define GICAME_PACK_END __attribute__((__packed__))
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)
#define GICAME_ISA_64
#else
#define GICAME_ISA_32
#endif

#if defined(GICAME_FORCE_IPC_SIZE_64)
    namespace Gicame { using ipc_size_t = uint64_t; };
#elif defined(GICAME_FORCE_IPC_SIZE_32)
    namespace Gicame { using ipc_size_t = uint32_t; };
#else
    namespace Gicame { using ipc_size_t = size_t; };
#endif

#define RUNTIME_ERROR(MSG) std::runtime_error(std::string(__FUNCTION__) + "(...): " + (MSG))

#if defined(GICAME_EXPORTS) || defined(GICAME_CRYPTO_EXPORTS) || defined(GICAME_COMMON_EXPORTS)
#define UNUSED(X) ((void)X)
#endif


#endif
