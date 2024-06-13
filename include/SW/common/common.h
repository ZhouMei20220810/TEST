/*
 * common.h
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/**
 * Type for specifying an error or status code.
 */
#ifdef _WIN32
typedef int         status_t;
#else
typedef int32_t     status_t;
#endif

/** FALSE */
#ifndef FALSE
#define FALSE 0
#endif
/** TRUE */
#ifndef TRUE
#define TRUE (!FALSE)
#endif

/**
 * AWE public API wrap for C++ compilers.
 */
#ifdef __cplusplus
#define AWE_BEGIN_DECLS     extern "C" {
#define AWE_END_DECLS       }
#else
#define AWE_BEGIN_DECLS
#define AWE_END_DECLS
#endif

#if defined(DOXYGEN) || !defined(_WIN32)
#define AWE_DECLARE(type)            type
#define AWE_DECLARE_NONSTD(type)     type
#define AWE_DECLARE_DATA
#elif defined(AWE_DECLARE_STATIC)
#define AWE_DECLARE(type)            type __stdcall
#define AWE_DECLARE_NONSTD(type)     type __cdecl
#define AWE_DECLARE_DATA
#elif defined(AWE_DECLARE_EXPORT)
#define AWE_DECLARE(type)            __declspec(dllexport) type __stdcall
#define AWE_DECLARE_NONSTD(type)     __declspec(dllexport) type __cdecl
#define AWE_DECLARE_DATA             __declspec(dllexport)
#else
#define AWE_DECLARE(type)            __declspec(dllimport) type __stdcall
#define AWE_DECLARE_NONSTD(type)     __declspec(dllimport) type __cdecl
#define AWE_DECLARE_DATA             __declspec(dllimport)
#endif


/*
 * Error codes.
 * All error codes are negative values.
 */
// Win32 #defines NO_ERROR as well.  It has the same value, so there's no
// real conflict, though it's a bit awkward.
#ifdef _WIN32
# undef NO_ERROR
#endif

enum{
    AWE_OK        = 0,
    AWE_NOERROR    = 0,
    AWE_UNKNOWN_ERROR        = (-2147483647-1), // INT32_MIN value
    AWE_NO_MEMORY            = -12, //-ENOMEM,
    AWE_INVALID_OPERATION    = -38, //-ENOSYS,
    AWE_BAD_VALUE            = -22, //-EINVAL,
    AWE_NAME_NOT_FOUND        = -2, //-ENOENT,
    AWE_PERMISSION_DENIED    = -1, //-EPERM,
    AWE_NO_INIT                = -19, //-ENODEV,
    AWE_ALREADY_EXISTS        = -17, //-EEXIST,
    AWE_DEAD_OBJECT            = -32, //-EPIPE,
    AWE_JPARKS_BROKE_IT        = -32, //-EPIPE,
    AWE_EBUSY                = -16, //-EBUSY
    AWE_TIMEDOUT            = -110, //-ETIMEDOUT
    AWE_EAGAIN                = -11, //-EAGAIN
    AWE_BAD_TYPE            = (AWE_UNKNOWN_ERROR + 1),
    AWE_BAD_PARAM            = (AWE_UNKNOWN_ERROR + 2),
    AWE_FAILED_TRANSACTION    = (AWE_UNKNOWN_ERROR + 3),
    AWE_EOF                    = (AWE_UNKNOWN_ERROR + 4),
};

// Restore define; enumeration is in "android" namespace, so the value defined
// there won't work for Win32 code in a different namespace.
#ifdef _WIN32
# define NO_ERROR 0L
#endif

//module status
enum {
    STATUS_NOP            = 0,
    STATUS_INIT            = 1,
    STATUS_STARTED        = 1 << 1,
    STATUS_RESOVLING    = 1 << 2,
    STATUS_CONNECTING1    = 1 << 3,
    STATUS_CONNECTED1    = 1 << 4,
    STATUS_CONNECTING2    = 1 << 5,
    STATUS_CONNECTED2    = 1 << 6,
    STATUS_ONLINE        = 1 << 7,
};

#define MODULE_STATUS_RESET(var, s)    var = (s)
#define MODULE_STATUS_SET(var, s)    ((var) |= (s))
#define MODULE_STATUS_CLR(var, s)    ((var) &= (~(s)))
#define MODULE_STATUS_ISSET(var, s)    ((var) & (s))

#endif /* COMMON_H_ */
