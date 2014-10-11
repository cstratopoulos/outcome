/* allocator_testing.hpp
Provides an allocator useful for unit testing exception safety
(C) 2014 Niall Douglas http://www.nedprod.com/
File Created: Aug 2014


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

#ifndef BOOST_ALLOCATOR_TESTING_HPP
#define BOOST_ALLOCATOR_TESTING_HPP

#include "local-bind-cpp-library/include/boost/config.hpp"
#include "local-bind-cpp-library/include/import.hpp"
#define BOOST_ALLOCATOR_TESTING (boost), (allocator_testing)
#define BOOST_ALLOCATOR_TESTING_STL11_IMPL std

#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_BEGIN        BOOST_LOCAL_BIND_NAMESPACE_BEGIN(BOOST_ALLOCATOR_TESTING, (stl11, inline))
#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_END          BOOST_LOCAL_BIND_NAMESPACE_END  (BOOST_ALLOCATOR_TESTING, (stl11, inline))
#include BOOST_LOCAL_BIND_INCLUDE_STL11(BOOST_ALLOCATOR_TESTING_STL11_IMPL, atomic)

namespace boost { namespace allocator_testing {

struct config
{
  atomic<size_t> count, fail_from, fail_at;
  config() : count(0), fail_from((size_t)-1), fail_at((size_t)-1) { }
};

static config &get_config(bool reset=false)
{
  static config c;
  if(reset)
  {
    c.count=0;
    c.fail_from=(size_t) -1;
    c.fail_at=(size_t) -1;
  }
  return c;
}
  
template<class T, class A=std::allocator<T>> struct allocator : public A
{
  template<class U> struct rebind { typedef allocator<U> other; };
  allocator() {}
  allocator(const allocator &o) : A(o) { }
  template<class U> allocator(const allocator<U> &o) : A(o) { }
  typename A::pointer allocate(typename A::size_type n, typename std::allocator<void>::const_pointer hint=0)
  {
    config &c=get_config();
    size_t count=++c.count;
    if(count>=c.fail_from || count==c.fail_at)
      throw std::bad_alloc();
    return A::allocate(n, hint);
  } 
};
  
} } // namespaces

#endif
