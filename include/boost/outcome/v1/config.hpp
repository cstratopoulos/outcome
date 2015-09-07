/* config.hpp
Configure Boost.Outcome with Boost.APIBind
(C) 2015 Niall Douglas http://www.nedprod.com/
File Created: August 2015


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

// Pull in detection of __MINGW64_VERSION_MAJOR
#ifdef __MINGW32__
# include <_mingw.h>
#endif

#include "../bindlib/include/import.h"
#include "../bindlib/include/boost/config.hpp"

#ifndef __cpp_alias_templates
# error Boost.Outcome needs template alias support in the compiler
#endif
#ifndef __cpp_variadic_templates
# error Boost.Outcome needs variadic template support in the compiler
#endif
#ifndef __cpp_noexcept
# error Boost.Outcome needs noexcept support in the compiler
#endif
#ifndef __cpp_constexpr
# error Boost.Outcome needs constexpr (C++ 11) support in the compiler
#endif

#if defined(BOOST_OUTCOME_LATEST_VERSION) && BOOST_OUTCOME_LATEST_VERSION < 1
# error You need to include the latest version of Boost.Outcome before any earlier versions within the same translation unit
#endif
#ifndef BOOST_OUTCOME_LATEST_VERSION
# define BOOST_OUTCOME_LATEST_VERSION 1
#endif

#undef BOOST_OUTCOME_V1_STL11_IMPL
#undef BOOST_OUTCOME_V1_ERROR_CODE_IMPL
#undef BOOST_OUTCOME_V1
#undef BOOST_OUTCOME_V1_NAMESPACE
#undef BOOST_OUTCOME_V1_NAMESPACE_BEGIN
#undef BOOST_OUTCOME_V1_NAMESPACE_END

// Default to the C++ 11 STL for atomic, chrono, mutex and thread except on Mingw32
#if (defined(BOOST_OUTCOME_USE_BOOST_THREAD) && BOOST_OUTCOME_USE_BOOST_THREAD) || (defined(__MINGW32__) && !defined(__MINGW64__) && !defined(__MINGW64_VERSION_MAJOR))
# define BOOST_OUTCOME_V1_STL11_IMPL boost
# ifndef BOOST_THREAD_VERSION
#  define BOOST_THREAD_VERSION 3
# endif
# if BOOST_THREAD_VERSION < 3
#  error Boost.Outcome requires that Boost.Thread be configured to v3 or later
# endif
#else
# define BOOST_OUTCOME_V1_STL11_IMPL std
# ifndef BOOST_OUTCOME_USE_BOOST_THREAD
#  define BOOST_OUTCOME_USE_BOOST_THREAD 0
# endif
#endif
#if BOOST_OUTCOME_USE_BOOST_ERROR_CODE
# define BOOST_OUTCOME_V1_ERROR_CODE_IMPL boost
# include "boost/thread/future.hpp"
#else
# define BOOST_OUTCOME_V1_ERROR_CODE_IMPL std
#endif

#if BOOST_OUTCOME_LATEST_VERSION == 1
# define BOOST_OUTCOME_V1 (boost), (outcome), (BOOST_BINDLIB_NAMESPACE_VERSION(v1, BOOST_OUTCOME_V1_STL11_IMPL, BOOST_OUTCOME_V1_ERROR_CODE_IMPL), inline)
#else
# define BOOST_OUTCOME_V1 (boost), (outcome), (BOOST_BINDLIB_NAMESPACE_VERSION(v1, BOOST_OUTCOME_V1_STL11_IMPL, BOOST_OUTCOME_V1_ERROR_CODE_IMPL))
#endif
#define BOOST_OUTCOME_V1_NAMESPACE       BOOST_BINDLIB_NAMESPACE      (BOOST_OUTCOME_V1)
#define BOOST_OUTCOME_V1_NAMESPACE_BEGIN BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_OUTCOME_V1)
#define BOOST_OUTCOME_V1_NAMESPACE_END   BOOST_BINDLIB_NAMESPACE_END  (BOOST_OUTCOME_V1)

#undef BOOST_OUTCOME_NEED_DEFINE
#undef BOOST_OUTCOME_NEED_DEFINE_DESCRIPTION
#if !BOOST_OUTCOME_USE_BOOST_THREAD && !BOOST_OUTCOME_USE_BOOST_ERROR_CODE
# ifndef BOOST_OUTCOME_NEED_DEFINE_00
#  define BOOST_OUTCOME_NEED_DEFINE_DESCRIPTION "BOOST_OUTCOME_USE_BOOST_THREAD=0 BOOST_OUTCOME_USE_BOOST_ERROR_CODE=0"
#  define BOOST_OUTCOME_NEED_DEFINE_00
#  define BOOST_OUTCOME_NEED_DEFINE 1
# endif
#elif BOOST_OUTCOME_USE_BOOST_THREAD && !BOOST_OUTCOME_USE_BOOST_ERROR_CODE
# ifndef BOOST_OUTCOME_NEED_DEFINE_10
#  define BOOST_OUTCOME_NEED_DEFINE_DESCRIPTION "BOOST_OUTCOME_USE_BOOST_THREAD=1 BOOST_OUTCOME_USE_BOOST_ERROR_CODE=0"
#  define BOOST_OUTCOME_NEED_DEFINE_10
#  define BOOST_OUTCOME_NEED_DEFINE 1
# endif
#elif !BOOST_OUTCOME_USE_BOOST_THREAD && BOOST_OUTCOME_USE_BOOST_ERROR_CODE
# ifndef BOOST_OUTCOME_NEED_DEFINE_01
#  define BOOST_OUTCOME_NEED_DEFINE_DESCRIPTION "BOOST_OUTCOME_USE_BOOST_THREAD=0 BOOST_OUTCOME_USE_BOOST_ERROR_CODE=1"
#  define BOOST_OUTCOME_NEED_DEFINE_01
#  define BOOST_OUTCOME_NEED_DEFINE 1
# endif
#elif BOOST_OUTCOME_USE_BOOST_THREAD && BOOST_OUTCOME_USE_BOOST_ERROR_CODE
# ifndef BOOST_OUTCOME_NEED_DEFINE_11
#  define BOOST_OUTCOME_NEED_DEFINE_DESCRIPTION "BOOST_OUTCOME_USE_BOOST_THREAD=1 BOOST_OUTCOME_USE_BOOST_ERROR_CODE=1"
#  define BOOST_OUTCOME_NEED_DEFINE_11
#  define BOOST_OUTCOME_NEED_DEFINE 1
# endif
#endif

#ifdef BOOST_OUTCOME_NEED_DEFINE
# undef BOOST_OUTCOME_FUTURE_H
# undef BOOST_OUTCOME_MONAD_H
# undef BOOST_OUTCOME_SPINLOCK_H
# undef BOOST_OUTCOME_TRIBOOL_H
# undef BOOST_OUTCOME_VALUE_STORAGE_H

// We need to prevent future_category, future_errc and future_error being bound in from BOOST_OUTCOME_USE_BOOST_THREAD
#define BOOST_STL11_FUTURE_MAP_NO_FUTURE_ERRC
#define BOOST_STL11_FUTURE_MAP_NO_FUTURE_ERROR
#define BOOST_STL11_FUTURE_MAP_NO_FUTURE_CATEGORY

#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_OUTCOME_V1)
#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_OUTCOME_V1)
#define BOOST_STL11_ATOMIC_MAP_NO_ATOMIC_CHAR32_T // missing VS14
#define BOOST_STL11_ATOMIC_MAP_NO_ATOMIC_CHAR16_T // missing VS14
#define BOOST_STL11_CHRONO_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_OUTCOME_V1, (stl11), (chrono))
#define BOOST_STL11_CHRONO_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_OUTCOME_V1, (stl11), (chrono))
#define BOOST_STL11_FUTURE_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_OUTCOME_V1, (stl11))
#define BOOST_STL11_FUTURE_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_OUTCOME_V1, (stl11))
#define BOOST_STL11_MUTEX_MAP_NAMESPACE_BEGIN         BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_OUTCOME_V1, (stl11))
#define BOOST_STL11_MUTEX_MAP_NAMESPACE_END           BOOST_BINDLIB_NAMESPACE_END  (BOOST_OUTCOME_V1, (stl11))
#define BOOST_STL11_SYSTEM_ERROR_MAP_NAMESPACE_BEGIN  BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_OUTCOME_V1, (stl11))
#define BOOST_STL11_SYSTEM_ERROR_MAP_NAMESPACE_END    BOOST_BINDLIB_NAMESPACE_END  (BOOST_OUTCOME_V1, (stl11))
#define BOOST_STL11_THREAD_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_OUTCOME_V1, (stl11))
#define BOOST_STL11_THREAD_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_OUTCOME_V1, (stl11))
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_OUTCOME_V1_STL11_IMPL, atomic)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_OUTCOME_V1_STL11_IMPL, chrono)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_OUTCOME_V1_STL11_IMPL, future)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_OUTCOME_V1_STL11_IMPL, mutex)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_OUTCOME_V1_ERROR_CODE_IMPL, system_error)
#include BOOST_BINDLIB_INCLUDE_STL11(../bindlib, BOOST_OUTCOME_V1_STL11_IMPL, thread)

BOOST_OUTCOME_V1_NAMESPACE_BEGIN
  namespace stl11
  {
#if BOOST_OUTCOME_USE_BOOST_ERROR_CODE
    using ::boost::future_category;
    using ::boost::future_errc;
    using ::boost::future_error;
#else
    using ::std::future_category;
    using ::std::future_errc;
    using ::std::future_error;
#endif
  }
  namespace chrono = stl11::chrono;
  namespace this_thread = stl11::this_thread;
  template<class T> using lock_guard = stl11::lock_guard<T>;
BOOST_OUTCOME_V1_NAMESPACE_END

#include <assert.h>
#include <vector>
#include <memory>
#include <array>

#ifdef BOOST_OUTCOME_ENABLE_VALGRIND
#include "valgrind/drd.h"
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_CREATE(p) ANNOTATE_RWLOCK_CREATE(p)
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_DESTROY(p) ANNOTATE_RWLOCK_DESTROY(p)
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_ACQUIRED(p, s) ANNOTATE_RWLOCK_ACQUIRED(p, s)
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_RELEASED(p, s) ANNOTATE_RWLOCK_RELEASED(p, s)
#define BOOST_OUTCOME_ANNOTATE_IGNORE_READS_BEGIN() ANNOTATE_IGNORE_READS_BEGIN()
#define BOOST_OUTCOME_ANNOTATE_IGNORE_READS_END() ANNOTATE_IGNORE_READS_END()
#define BOOST_OUTCOME_ANNOTATE_IGNORE_WRITES_BEGIN() ANNOTATE_IGNORE_WRITES_BEGIN()
#define BOOST_OUTCOME_ANNOTATE_IGNORE_WRITES_END() ANNOTATE_IGNORE_WRITES_END()
#define BOOST_OUTCOME_DRD_IGNORE_VAR(x) DRD_IGNORE_VAR(x)
#define BOOST_OUTCOME_DRD_STOP_IGNORING_VAR(x) DRD_STOP_IGNORING_VAR(x)
#define BOOST_OUTCOME_RUNNING_ON_VALGRIND RUNNING_ON_VALGRIND
#else
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_CREATE(p)
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_DESTROY(p)
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_ACQUIRED(p, s)
#define BOOST_OUTCOME_ANNOTATE_RWLOCK_RELEASED(p, s)
#define BOOST_OUTCOME_ANNOTATE_IGNORE_READS_BEGIN()
#define BOOST_OUTCOME_ANNOTATE_IGNORE_READS_END()
#define BOOST_OUTCOME_ANNOTATE_IGNORE_WRITES_BEGIN()
#define BOOST_OUTCOME_ANNOTATE_IGNORE_WRITES_END()
#define BOOST_OUTCOME_DRD_IGNORE_VAR(x)
#define BOOST_OUTCOME_DRD_STOP_IGNORING_VAR(x)
#define BOOST_OUTCOME_RUNNING_ON_VALGRIND (0)
#endif

#if ! defined BOOST_OUTCOME_RELAXED_CONSTEXPR
# ifdef __cpp_relaxed_constexpr
#  define BOOST_OUTCOME_RELAXED_CONSTEXPR constexpr
# endif
#endif
#ifndef BOOST_OUTCOME_RELAXED_CONSTEXPR
# define BOOST_OUTCOME_RELAXED_CONSTEXPR
#endif

#if !defined BOOST_OUTCOME_NOINLINE
# define BOOST_OUTCOME_NOINLINE BOOST_NOINLINE
#endif

#if !defined BOOST_OUTCOME_FORCEINLINE
# define BOOST_OUTCOME_FORCEINLINE BOOST_FORCEINLINE
#endif

#ifndef BOOST_OUTCOME_IN_THREAD_SANITIZER
# if defined(__has_feature)
#  if __has_feature(thread_sanitizer)
#   define BOOST_OUTCOME_IN_THREAD_SANITIZER 1
#  endif
# elif defined(__SANITIZE_ADDRESS__)
#  define BOOST_OUTCOME_IN_THREAD_SANITIZER 1
# endif
#endif
#ifndef BOOST_OUTCOME_IN_THREAD_SANITIZER
# define BOOST_OUTCOME_IN_THREAD_SANITIZER 0
#endif

#ifndef BOOST_OUTCOME_ALIGN
# ifdef __cpp_alignas
#  define BOOST_OUTCOME_ALIGN(n) alignas(n)
# elif defined(__GNUC__)
#  define BOOST_OUTCOME_ALIGN(n) __attribute__((aligned(n)))
# elif defined(_MSC_VER)
#  define BOOST_OUTCOME_ALIGN(n) __declspec(align(n))
# else
#  define BOOST_OUTCOME_ALIGN(n)
# endif
#endif


// For dump
#include <algorithm>
#include <istream>
#include <ostream>


// Turn this on if you have a compiler which understands __transaction_relaxed
//#define BOOST_HAVE_TRANSACTIONAL_MEMORY_COMPILER


// For some odd reason, VS2015 really hates to do much inlining unless forced
#ifdef _MSC_VER
//# pragma inline_depth(255)
//# pragma inline_recursion(on)
# define BOOST_OUTCOME_FUTURE_CXX14_CONSTEXPR BOOST_FORCEINLINE
# define BOOST_OUTCOME_FUTURE_MSVC_HELP BOOST_FORCEINLINE
#else
# define BOOST_OUTCOME_FUTURE_CXX14_CONSTEXPR BOOST_CXX14_CONSTEXPR
# define BOOST_OUTCOME_FUTURE_MSVC_HELP 
#endif

#endif  // BOOST_OUTCOME_NEED_DEFINE
