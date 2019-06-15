#ifndef COMPILER_H
#define COMPILER_H

/*
    Macros to simple work with GCC

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h> /* offsetof */
#include <stdint.h>
#include <string.h> /* memcpy */

/* use thic macro to convert non-Clang "word" to string  */
#define ____tostring____(s) #s
#define tostring(s) ____tostring____(s)

/* CONCAT use this macro to concat non-Clang "words" in one string */
#define ____concat____(x, y) x ## y
#define concat(x, y) ____concat____(x, y)

/* use this macro, to get unique name for your temporary variable */
#define unique_var_name(name) concat(concat(UNIQUE_VAR_, name), __COUNTER__)

/* set if () { } at the begining of pipeline */
#define likely(x) __builtin_expect(!!(x), 1)

/* set if () { } at the end of pipeline */
#define unlikely(x) __builtin_expect(!!(x), 0)

/* to avoid warning about uninit var */
#define init_var(x) ((x) = ((typeof(x)){0}))

/* use this macro to get parent struct from member struct */
#define container_of(ptr, type, member) \
    __extension__ \
    ({ \
        const typeof(((type *)0)->member) *______mptr = (ptr); \
        (type *) ((char *) ______mptr - offsetof(type, member)); \
    })

/* use this macro to assert types */
#define type_check(t1, t2) (!((t1 *)0 - (t2 *)0))

/* use this macro to assert types of variables */
#define type_var_check(v1, v2) type_check(typeof(v1), typeof(v2))

#define __unused(x) (void)(x)

/* use this macros if casting to (void) is ugly for you */
#define unused_param(x)  __unused(x)
#define unused_retval(x) do { if (unlikely((x))) { } } while (0)
#define unused_value(x)  __unused(x)
#define unused(x)        __unused(x)

/* BUILTIN FUNC */

/* get index of least bit ( from left ) or -1 iff X == 0  */
#define least_1(x) \
    __extension__ \
    ({ \
        int ________x = (int)(x); \
        if (sizeof(x) == sizeof(int)) \
            ________x = least_1_int((int)x); \
        else if (sizeof(x) == sizeof(long)) \
            ________x = least_1_long((long)x); \
        else if (sizeof(x) == sizeof(long long)) \
            ________x = least_1_longlong((long long)x); \
        ________x; \
    })

#define least_1_int(x)          (__builtin_ffs(x) - 1)
#define least_1_long(x)         (__builtin_ffsl(x) - 1)
#define least_1_longlong(x)     (__builtin_ffsll(x) - 1)

/* get number of leading zeros */
#define leading_0(x) \
    __extension__ \
    ({ \
        int ________x = (int)(x); \
        if (sizeof(x) == sizeof(unsigned int)) \
            ________x = leading_0_int((unsigned int)x); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = leading_0_long((unsigned long)x); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = leading_0_longlong((unsigned long long)x); \
        ________x; \
    })

#define leading_0_int(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned int); \
        (______x) ? (int)__builtin_clz(______x) : (int)sizeof(typeof(______x)) << 3; \
    })

#define leading_0_long(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long); \
        (______x) ? (int)__builtin_clzl(______x) : (int)sizeof(typeof(______x)) << 3; \
    })

#define leading_0_longlong(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long long); \
        (______x) ? (int)__builtin_clzll(______x) : (int)sizeof(typeof(______x)) << 3; \
    })


/* get number of trailing zeros */
#define trailing_0(x) \
    __extension__ \
    ({ \
        int ________x = (int)(x); \
        if (sizeof(x) == sizeof(unsigned int)) \
            ________x = trailing_0_int((unsigned int)x); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = trailing_0_long((unsigned long)x); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = trailing_0_longlong((unsigned long long)x); \
        ________x; \
    })

#define trailing_0_int(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned int); \
        (______x) ? (int)__builtin_ctz(______x) : (int)sizeof(typeof(______x)) << 3; \
    })

#define trailing_0_long(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long); \
        (______x) ? (int)__builtin_ctzl(______x) : (int)sizeof(typeof(______x)) << 3; \
    })

#define trailing_0_longlong(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long long); \
        (______x) ? (int)__builtin_ctzll(______x) : (int)sizeof(typeof(______x)) << 3; \
    })

/* get number of ones */
#define number_1(x) \
    __extension__ \
    ({ \
        int ________x = (int)(x); \
        if (sizeof(x) == sizeof(unsigned int)) \
            ________x = number_1_int((unsigned int)x); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = number_1_long((unsigned long)x); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = number_1_longlong((unsigned long long)x); \
        ________x; \
    })

#define number_1_int(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned int); \
        (______x) ? (int)__builtin_popcount(______x) : 0; \
    })

#define number_1_long(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long); \
        (______x) ? (int)__builtin_popcountl(______x) : 0; \
    })

#define number_1_longlong(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long long); \
        (______x) ? (int)__builtin_popcountll(______x) : 0; \
    })

/* get parity number of 1 ( number of 1 % 2) */
#define parity_1(x) \
    __extension__ \
    ({ \
        int ________x = (int)(x); \
        if (sizeof(x) == sizeof(unsigned int)) \
            ________x = parity_1_int((unsigned int)x); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = parity_1_long((unsigned long)x); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = parity_1_longlong((unsigned long long)x); \
        ________x; \
    })

#define parity_1_int(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned int); \
        !!!__builtin_parity(______x); \
    })

#define parity_1_long(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long); \
        !!!__builtin_parityl(______x); \
    })

#define parity_1_longlong(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), unsigned long long); \
        !!!__builtin_parityll(______x); \
    })

/* swap bytes in variable */
#define swap_bytes16(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), uint16_t); \
        __builtin_bswap16(______x); \
    })

#define swap_bytes32(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), uint32_t); \
        __builtin_bswap32(______x); \
    })

#define swap_bytes64(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        (void)type_check(typeof(______x), uint64_t); \
        __builtin_bswap64(______x); \
    })

/* alloc on stack by resize stack frame in init f call */
#define alloc_on_stack(size) __builtin_alloca(size)

/* alloc with align on stack by resize stack frame in f init call  */
#define alloc_on_stack_align(size, align) __builtin_alloca_with_align(size, align)

/* tell compiler that this code is unreachable */
#define unreachable  __builtin_unreachable

/* clear cache */
#define clear_cache(begin, end) __builtin___clear_cache((char *)begin, (char *)end)

#define CACHE_WRITE 1
#define CACHE_READ  0

#define CACHE_NO_SAVE           0
#define CACHE_SAVE_LOW_PRIO     1
#define CACHE_SAVE_NORMAL_PRIO  2
#define CACHE_SAVE_HIGH_PRIO    3

#define load_to_cache(addr, rw, prio)  __builtin_prefetch(addr, rw, prio)

#define sw_mem_barrier()  asm volatile ("" : : : "memory")
#define hw_mem_barrier()  __sync_synchronize()

/* atomic in sense of HW read / write not in thread sense */
#define atomic(x) \
    do { \
        hw_mem_barrier(); \
        x; \
        hw_mem_barrier(); \
    } while (0)

/* use thic macro if you want to write data of @size from  @src to @dst */
#define WRITE_ONCE_SIZE(dst, src, size) \
    do{ \
        _Pragma("GCC diagnostic push"); \
        _Pragma("GCC diagnostic ignored \"-Wstrict-aliasing\""); \
        (void)type_var_check(dst, src); \
        switch (size) \
        { \
            case 1: *(uint8_t *)&dst = *(uint8_t *)&src; break; \
            case 2: *(uint16_t *)&dst = *(uint16_t *)&src; break; \
            case 4: *(uint32_t *)&dst = *(uint32_t *)&src; break; \
            case 8: *(uint64_t *)&dst = *(uint64_t *)&src; break; \
            default: (void)memcpy((void *)&dst, (void *)&src, (size_t)(size)); \
        } \
        _Pragma("GCC diagnostic pop"); \
    } while (0)

/* write once when variables have strict types, like int, struct s, not *(int *)char[20] */
#define WRITE_ONCE(dst, src) WRITE_ONCE_SIZE(dst, src, sizeof(dst))

/* use thic macro if you want to read data of @size from  @src to @dst  */
#define READ_ONCE_SIZE(dst, src, size) WRITE_ONCE_SIZE(dst, src, size)
#define READ_ONCE(dst, src) READ_ONCE_SIZE(dst, src, sizeof(dst))

/* writing in  HW sync mode */
#define WRITE_ONCE_SIZE_SYNC(dst, src, size) atomic(WRITE_ONCE_SIZE(dst, src, size))
#define WRITE_ONCE_SYNC(dst, src) WRITE_ONCE_SIZE_SYNC(dst, src, sizeof(dst))

/* reading in HW sync mode */
#define READ_ONCE_SIZE_SYNC(dst, src, size) atomic(READ_ONCE_SIZE(dst, src, size))
#define READ_ONCE_SYNC(dst, src) READ_ONCE_SIZE_SYNC(dst, src, sizeof(dst))

/* writing val like 3, 8ULL */
#define WRITE_ONCE_VAL(dst, val) \
    do { \
        typeof(val) ________val = (val); \
        WRITE_ONCE(dst, ________val); \
    } while (0)

#define WRITE_ONCE_VAL_SYNC(dst, val) \
    do { \
        typeof(val) ________val = (val); \
        WRITE_ONCE_SYNC(dst, ________val); \
    } while (0)

/* reading val like 3, 8ULL */
#define READ_ONCE_VAL(dst, val) WRITE_ONCE_VAL(dst, val)
#define READ_ONCE_VAL_SYNC(dst, val) WRITE_ONCE_VAL_SYNC(dst, val)

/* FUNCTION ATTR */

/* use this macro instead of inline */
#define ___inline___ inline __attribute__(( always_inline ))

/* function can't return, it's exit immediately  */
#define ___no_return___ __attribute__(( noreturn ))

/* f args can't be null */
#define ___nonull___(...) __attribute__(( nonnull(__VA_ARGS__) ))

/* f is used very few times, so put it in special part of text */
#define ___unlikely_use___ __attribute__(( cold ))

/*f is used so many times so put it at the begining of text */
#define ___likely_use___ __attribute__(( hot ))

#define PRIO_RESERVED 100

/* f is call before main ( init global data in this f ) prio >= 0 */
#define ___before_main___(prio) __attribute__(( constructor(prio + PRIO_RESERVED + 1) ))

/* f is call after main or in exit() (dealloc global data in this f) prio >= 0 */
#define ___after_main___(prio) __attribute__(( destructor(prio + PRIO_RESERVED + 1) ))

/* this function could be compiled in few version with smid support */
#define ___simd___ __attribute__(( simd ))

/* tell compiler that this f or var could be not used */
#define ___unused___ __attribute__(( unused ))

/* tell compiler that function is weak and can be overwrite */
#define ___weak___ __attribute__(( weak ))

#define ___compile_time_check_cond___(cond) ((void)sizeof(char[1 - 2 * cond]));

#ifdef static_assert
#undef static_assert
#endif

/* use this macro to get compile time error when function will be used */
#define ___error___(msg) __attribute__(( error(msg) ));

/* use this macro to get compile time warning when function will be used */
#define ___warning___(msg) __attribute__(( warning(msg) ));

/* use this macro to make check in compile time (like assert, but works during commpilation on compile time known conds) */
#define static_assert(cond) __static_assert(cond, __LINE__)
#define __static_assert(cond, name) \
    do { \
        const int ______val = !(cond); \
        extern void concat(error_, name)(void) ___error___(tostring(cond) " FAILED!!!"); \
        if (______val) \
            concat(error_, name)(); \
        ___compile_time_check_cond___(______val); \
    } while (0)

/* VARIABLE ATTR */

/* align variable to bytes */
#define ___align___(bytes) __attribute__(( aligned(bytes) ))

/* align to biggest aligment */
#define ___max_align___(bytes) __attribute__((aligned (__BIGGEST_ALIGNMENT__) ))

/* packed variable / structure */
#define ___packed___ __attribute__(( packed ))

/* garbage_collected by function with only 1 arg: pointer to this variable */
#define ___garbage_collector___(func) __attribute__(( cleanup(func) ))

/* use this instead of restrict */
#define ___restrict___ restrict

#define ___nobreak___ __attribute__(( fallthrough ))

#endif
