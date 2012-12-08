/* ex: set ts=4 et: */
/*
 * Portable definitions for byte-swapping
 *
 * Copyright (c) 2012 Ryan Flynn <parseerror+bswap@gmail.com> <parseerror.com> <rflynn.com>
 *
 * uppercase macros suitable for compile-time constants
 * lowercase functions for run-time
 *
 * byte-swapping:
 *  BSWAP32(x), BSWAP64(x)
 *  bswap32(x), bswap64(x)
 *
 * endianness:
 *  LITTLEENDIAN32(x), LITTLEENDIAN64(x)
 *  BIGENDIAN32(x), BIGENDIAN64(x)
 */

#ifndef BSWAP_H
#define BSWAP_H

/* use efficient builtins if possible... */

#if defined(__GNUC__) || defined(__clang__)
#   ifdef __has_builtin
#       if __has_builtin(__builtin_bswap32)
#           define BSWAP32(x) __builtin_bswap32(x)
#       endif
#       if __has_builtin(__builtin_bswap64)
#           define BSWAP64(x) __builtin_bswap64(x)
#       endif
#   endif
#elif defined(__INTEL_COMPILER)
    /* TODO: */
#endif

/* fall back to doing things the hard (slow) way */

#ifndef BSWAP32
#define BSWAP32(x)                                     \
    ((((uint32_t)(x) & 0x000000FF) << 24) |            \
     (((uint32_t)(x) & 0x0000FF00) << 8)  |            \
     (((uint32_t)(x) >> 8) & 0x0000FF00)  |            \
     (((uint32_t)(x) >> 24) & 0x000000FF))
#endif

#ifndef BSWAP64
#define BSWAP64(x)                                     \
    (((uint64_t)(x) << 56) |                           \
     (((uint64_t)(x) << 40) & 0X00FF000000000000ULL) | \
     (((uint64_t)(x) << 24) & 0X0000FF0000000000ULL) | \
     (((uint64_t)(x) << 8)  & 0X000000FF00000000ULL) | \
     (((uint64_t)(x) >> 8)  & 0X00000000FF000000ULL) | \
     (((uint64_t)(x) >> 24) & 0X0000000000FF0000ULL) | \
     (((uint64_t)(x) >> 40) & 0X000000000000FF00ULL) | \
     ((uint64_t)(x)  >> 56))
#endif

#ifdef WIN32 /* assume Visual Studio */
#   define WIN32_LEAN_AND_MEAN
#   include <winsock2.h>

#   ifdef _DEBUG

/* ref: http://msdn.microsoft.com/en-us/library/a3140177(v=vs.80).aspx */
#       include <stdlib.h>
#       define bswap32 _byteswap_ulong
#       define bswap64 _byteswap_uint64

#   else

__inline DWORD bswap32(DWORD x)
{
  __asm {
    mov   edx, [ebp+8]
    bswap edx
  }
}

__inline DWORDLONG bswap64(DWORDLONG x)
{
  __asm {
    mov   edx, [ebp+8]
    mov   eax, [ebp+12]
    bswap edx
    bswap eax
  }
}
#   endif

#endif

#ifdef __BYTE_ORDER
#   if __BYTE_ORDER == __BIG_ENDIAN
#       define BSWAP_ENDIAN BIG
#   elif __BYTE_ORDER == __LITTLE_ENDIAN
#       define BSWAP_ENDIAN LITTLE
#   endif
#elif defined(__BIG_ENDIAN__)
#   define BSWAP_ENDIAN BIG
#else
#   define BSWAP_ENDIAN LITTLE /* PC-centric fallback */
#endif

#if BSWAP_ENDIAN == LITTLE
#   define BIGENDIAN32(x)    BSWAP32(x)
#   define BIGENDIAN64(x)    BSWAP64(x)
#   define LITTLEENDIAN32(x) (x)
#   define LITTLEENDIAN64(x) (x)
#elif BSWAP_ENDIAN == BIG
#   define BIGENDIAN32(x)    (x)
#   define BIGENDIAN64(x)    (x)
#   define LITTLEENDIAN32(x) BSWAP32(x)
#   define LITTLEENDIAN64(x) BSWAP64(x)
#endif

#endif
