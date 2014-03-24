/*This code copyrighted (2013) for the Warzone 2100 Legacy Project under the GPLv2.

Warzone 2100 Legacy is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Warzone 2100 Legacy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Warzone 2100 Legacy; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA*/
/*! \file
 *  \brief Simple type definitions.
 */

#ifndef __INCLUDED_LIB_FRAMEWORK_TYPES_H__
#define __INCLUDED_LIB_FRAMEWORK_TYPES_H__

#include "wzglobal.h"

#ifdef HAVE_INTTYPES_H // defined WZ_C99
/* Compilers that have support for C99 have all values below defined in stdint.h */
# include <inttypes.h>
#else
// Defines C99 types for C99 incompatible compilers (e.g. MSVC)
#include <SDL_stdinc.h>
# define INT8_MIN               (-128)
# define INT16_MIN              (-32767-1)
# define INT32_MIN              (-2147483647-1)
# define INT8_MAX               (127)
# define INT16_MAX              (32767)
# define INT32_MAX              (2147483647)
# define UINT8_MAX              (255)
# define UINT16_MAX             (65535)
# define UINT32_MAX             (4294967295U)
#ifdef WZ_CC_MSVC
#define PRIu64  "I64u"
#endif
#endif // WZ_C99

#include <limits.h>
#include <ctype.h>

/* Basic numeric types */
typedef uint8_t  uint8_t;
typedef int8_t   int8_t;
typedef uint16_t uint16_t;
typedef int16_t  int16_t;
typedef uint32_t uint32_t;
typedef int32_t  int32_t;

/* Numeric size defines */
#define uint8_t_MAX	UINT8_MAX
#define int8_t_MIN	INT8_MIN
#define int8_t_MAX	INT8_MAX
#define uint16_t_MAX	UINT16_MAX
#define int16_t_MIN	INT16_MIN
#define int16_t_MAX	INT16_MAX
#define uint32_t_MAX	UINT32_MAX
#define int32_t_MIN	INT32_MIN
#define int32_t_MAX	INT32_MAX

// If we are C99 compatible, the "bool" macro will be defined in <stdbool.h> (as _Bool)
// C++ comes with an integrated bool type
#if defined(WZ_CXX98)
#elif defined(WZ_C99)
# include <stdbool.h>
#else
// Pretend we are C99 compatible (well, for the bool type then)
# ifndef bool
#  define bool BOOL
# endif
# ifndef true
#  define true (1)
# endif
# ifndef false
#  define false (0)
# endif
# ifndef __bool_true_false_are_defined
#  define __bool_true_false_are_defined (1)
# endif
#endif /* WZ_C99 */

#if !defined(WZ_OS_WIN) && !defined(WZ_NO_LCAPS_BOOL)
typedef int BOOL;
#endif // WZ_OS_WIN

#endif // __INCLUDED_LIB_FRAMEWORK_TYPES_H__
