#pragma once
// @note: base layer single header file,
//        created by Mohamed Touiti
// for more info visit: https://sourceforge.net/p/predef/wiki/Compilers/

#if defined(__clang__)
    // clang compiler
    #define COMPILER_CLANG 1

    // platform detection
    #if defined(_WIN32)
        #define OS_WINDOWS 1
    #elif defined(__gnu_linux__)
        #define OS_LINUX 1
    #elif defined(__APPLE__) && defined(__MACH__)
        #define OS_MAC 1
    #else
        #error missing OS detection
    #endif

    #if defined(__amd64__)
        #define ARCH_X64 1
    #elif defined(__i386__)
        #define ARCH_X86 1
    #elif defined(__arm__)
        #define ARCH_ARM 1
    #elif defined(__aarch64__)
        #define ARCH_ARM64 1
    #else
        #error missing ARCH detection
    #endif
#elif defined(_MSC_VER)
    // microsoft compiler
    #define COMPILER_CL 1

    // platform detection
    #if defined(_WIN32)
        #define OS_WINDOWS 1
    #else
        #error missing OS detection
    #endif

    #if defined(_M_AMD64)
        #define ARCH_X64 1
    #elif defined(_M_I86)
        #define ARCH_X86 1
    #elif defined(_M_ARM)
        #define ARCH_ARM 1
    #else
        #error missing ARCH detection
    #endif
#elif defined(__GNUC__)
    // gcc compiler
    #define COMPILER_GCC 1

    // platform detection
    #if defined(_WIN32)
        #define OS_WINDOWS 1
    #elif defined(__gnu_linux__)
        #define OS_LINUX 1
    #elif defined(__APPLE__) && defined(__MACH__)
        #define OS_MAC 1
    #else
        #error missing OS detection
    #endif

    #if defined(__amd64__)
        #define ARCH_X64 1
    #elif defined(__i386__)
        #define ARCH_X86 1
    #elif defined(__arm__)
        #define ARCH_ARM 1
    #elif defined(__aarch64__)
        #define ARCH_ARM64 1
    #else
        #error missing ARCH detection
    #endif
#else
    // unknown compiler
    #error no context was determined for the this compiler
#endif

// resolve compiler context
#if !defined(COMPILER_CL)
    #define COMPILER_CL 0
#endif
#if !defined(COMPILER_CLANG)
    #define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_GCC)
    #define COMPILER_GCC 0
#endif

// resolve os context
#if !defined(OS_WINDOWS)
    #define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
    #define OS_LINUX 0
#endif
#if !defined(OS_MAC)
    #define OS_MAC 0
#endif

#if !defined(ARCH_X64)
    #define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
    #define ARCH_X86 0
#endif
#if !defined(ARCH_ARM)
    #define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
    #define ARCH_ARM64 0
#endif

#include <stdint.h>

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;


#define null 0
#define size_of(x) ((s64)sizeof(x))

#define BIT(x) (1 << (x))
#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) ((x) << 40)

#define UNUSED(x) ((void)(x))

#define array_size(arr) (sizeof(arr) / sizeof((arr)[0]))

#if OS_WINDOWS
#if COMPILER_GCC
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() __debugbreak()
#endif
#elif OS_LINUX
#define DEBUG_BREAK() __builtin_debugtrap()
#elif OS_MAC
#define DEBUG_BREAK() __builtin_trap()
#endif

#if !defined(FORCE_INLINE)
    #if COMPILER_CL
        #define FORCE_INLINE __forceinline
    #else
        #define FORCE_INLINE __attribute__ ((__always_inline__))
    #endif
#endif

// define your custom logger
#ifndef log
#define log(message, ...)
#endif

// define your custom error logger
#ifndef log_error
#define log_error(message, ...)
#endif

#if ENABLE_ASSERTS
#define assert(exp)\
do { \
    if (!(exp)) \
    { \
        log_error("Assertion failed: %s at %s:%d\n", #exp, __FILE__, __LINE__); \
        DEBUG_BREAK(); \
    } \
} while (0)
#else
#define assert(exp)
#endif

static void panic(void);

#define STR_FMT "%.*s"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp(x, a, b) (((x) < (a)) ? (a) : ((x) > (b)) ? (b) : (x))

#define pointer_to_int(ptr) (u64)((u8 *)(ptr) - (u8 *)0)

#define member_of(Type, m) (((Type *)0)->m)
#define offset_of(Type, m) pointer_to_int(&member_of(Type, m))

#define stringify_(str) #str
#define stringify(str) stringify_(str)

#define glue_(a, b) a##b
#define glue(a, b) glue_(a, b)

#ifndef DEFAULT_MEMORY_ALIGNMENT
#define DEFAULT_MEMORY_ALIGNMENT (2 * size_of(void *))
#endif


constexpr s8 MIN_S8 = 0x80; // -128
constexpr s8 MAX_S8 = 0x7f; // 127

constexpr s16 MIN_S16 = 0x8000; // -32768
constexpr s16 MAX_S16 = 0x7fff; // 32767

constexpr s32 MIN_S32 = 0x80000000; // -2147483648
constexpr s32 MAX_S32 = 0x7fffffff; // 2147483647

constexpr s64 MIN_S64 = 0x8000000000000000llu; // -2^63
constexpr s64 MAX_S64 = 0x7fffffffffffffffllu; // 2^63 - 1

constexpr u8 MAX_U8 = 0xff; // 255
constexpr u16 MAX_U16 = 0xffff; // 65535
constexpr u32 MAX_U32 = 0xffffffff; // 4294967295
constexpr u64 MAX_U64 = 0xffffffffffffffff; // 2^64 - 1

constexpr f32 F32_MIN = 1.17549435e-38f;
constexpr f32 F32_MAX = 3.40282347e+38f;

constexpr f64 F64_MIN  = 2.2250738585072014e-308;
constexpr f64 F64_MAX = 1.7976931348623157e+308;


enum OperatingSystem
{
    OPERATING_SYSTEM_NULL,
    OPERATING_SYSTEM_WINDOWS,
    OPERATING_SYSTEM_LINUX,
    OPERATING_SYSTEM_MAC,

    OPERATING_SYSTEM_COUNT,
};

enum Architecture
{
    ARCHITECTURE_NULL,
    ARCHITECTURE_X64,
    ARCHITECTURE_X86,
    ARCHITECTURE_ARM,
    ARCHITECTURE_ARM64,

    ARCHITECTURE_COUNT,
};

//////////////////////////////////////////
// symbolic const functions

static OperatingSystem operating_system_from_context(void);
static Architecture architecture_from_context(void);

static char *operating_system_to_string(OperatingSystem os);
static char *architecture_to_string(Architecture arch);


enum AllocatorMode
{
    ALLOCATOR_ALLOCATE,
    ALLOCATOR_RESIZE,
    ALLOCATOR_FREE,
    ALLOCATOR_FREE_ALL
};

typedef void *(*AllocatorProc)(AllocatorMode mode, s64 size, s64 old_size, void *old_memory, void *allocator_data, s64 options);
#define ALLOCATOR_PROC(name) void *name(AllocatorMode mode, s64 size, s64 old_size, void *old_memory, void *allocator_data, s64 options)

struct Allocator
{
    AllocatorProc proc;
    void *data;
};

static ALLOCATOR_PROC(default_allocator);


#define alloc_size(size) default_allocator(ALLOCATOR_ALLOCATE, size, 0, null, null, 0);
#define alloc_struct(Type) (Type *)default_allocator(ALLOCATOR_ALLOCATE, sizeof(Type), 0, null, null, 0)
#define alloc_array(Type, count) (Type *)default_allocator(ALLOCATOR_ALLOCATE, sizeof(Type) * (count), 0, null, null, 0)
#define alloc_free(memory) (void)default_allocator(ALLOCATOR_FREE, 0, 0, memory, null, 0)



// implementation

static OperatingSystem operating_system_from_context(void)
{
    OperatingSystem result = OPERATING_SYSTEM_NULL;
#if OS_WINDOWS
    result = OPERATING_SYSTEM_WINDOWS;
#elif OS_LINUX
    result = OPERATING_SYSTEM_LINUX;
#elif OS_MAC
    result = OPERATING_SYSTEM_MAC;
#endif

    return result;
}

static Architecture architecture_from_context(void)
{
    Architecture result = ARCHITECTURE_NULL;
#if ARCH_X64
    result = ARCHITECTURE_X64;
#elif ARCH_X86
    result = ARCHITECTURE_X86;
#elif ARCH_ARM
    result = ARCHITECTURE_ARM;
#elif ARCH_ARM64
    result = ARCHITECTURE_ARM64;
#endif

    return result;
}

static char *operating_system_to_string(OperatingSystem os)
{
    char *result = "(null)";
    switch (os)
    {
        case OPERATING_SYSTEM_WINDOWS:
        {
            result = "Windows";
        } break;

        case OPERATING_SYSTEM_LINUX:
        {   
            result = "Linux";
        } break;

        case OPERATING_SYSTEM_MAC:
        {
             result = "Mac";
        } break;

        default: break;
    }

    return result;
}

static char *architecture_to_string(Architecture arch)
{
    char *result = "(null)";
    switch (arch)
    {
        case ARCHITECTURE_X64:
        {
            result = "x64";
        } break;
        
        case ARCHITECTURE_X86:
        {
            result = "x86";
        } break;
        
        case ARCHITECTURE_ARM:
        {
            result = "arm";
        } break;
        
        case ARCHITECTURE_ARM64:
        {
            result = "arm64";
        } break;
        
        default: break;
    }

    return result;
}

// @todo: move this away (allocators.cpp maybe?)
#include <stdlib.h>
#include <string.h>

static ALLOCATOR_PROC(default_allocator)
{
    switch (mode)
    {
        case ALLOCATOR_ALLOCATE:
            return malloc(size);
        
        case ALLOCATOR_RESIZE:
        {
            // for now, we dont even bother with realloc, just allocate, copy, free.
            void *result = malloc(size);
            if (result == null) return null;

            if (old_memory && (old_size > 0))
            {
                memcpy(result, old_memory, old_size);
                free(old_memory);
            }

            return result;
        } break;

        case ALLOCATOR_FREE:
        {
            free(old_memory);
            return null;
        } break;

        case ALLOCATOR_FREE_ALL:
        {
            // @incomplete: not supported while we are using C malloc...
            // use Windows heaps or else an off-the-shelf malloc that gives us control.

            // @incomplete: Panic.
            assert(false);
            panic();
            return null;
        } break;

        default:
        {
            // @incomplete: Panic.
            assert(false);
            panic();
            return null;
        } break;
    }
}

static void panic(void)
{
    abort();
}

static inline bool is_power_of_two(u64 x)
{
    return (x & (x-1)) == 0;
}

static inline u64 align_size(u64 size, u64 alignment)
{
    u64 result = size + alignment - 1;
    return result - (result % alignment);
}

static FORCE_INLINE u64 align_forward(void *ptr, u64 alignment)
{
    assert(is_power_of_two(alignment));

    u64 p = (u64)ptr;
    // p % alignment
    u64 modulo = p & (alignment - 1);

    if (modulo != 0)
    {
        // if the address isn't aligned, seek the next aligned value
        p += alignment - modulo;
    }
    return p;
}
