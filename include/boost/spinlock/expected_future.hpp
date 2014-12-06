/* expected_future.hpp
Provides a non-allocating expected<T, E> based future-promise
(C) 2014 Niall Douglas http://www.nedprod.com/
File Created: Oct 2014


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

#ifndef BOOST_EXPECTED_FUTURE_HPP
#define BOOST_EXPECTED_FUTURE_HPP

#include "spinlock.hpp"
#include "expected/include/boost/expected/expected.hpp"

#include "bindlib/include/import.h"
#ifndef BOOST_EXPECTED_FUTURE_V1_STL11_IMPL
#define BOOST_EXPECTED_FUTURE_V1_STL11_IMPL std
#endif
#define BOOST_EXPECTED_FUTURE_V1 (boost), (expected_future), (BOOST_BINDLIB_NAMESPACE_VERSION(v1, BOOST_EXPECTED_FUTURE_V1_STL11_IMPL), inline)
#define BOOST_EXPECTED_FUTURE_V1_NAMESPACE       BOOST_BINDLIB_NAMESPACE      (BOOST_EXPECTED_FUTURE_V1)
#define BOOST_EXPECTED_FUTURE_V1_NAMESPACE_BEGIN BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_EXPECTED_FUTURE_V1)
#define BOOST_EXPECTED_FUTURE_V1_NAMESPACE_END   BOOST_BINDLIB_NAMESPACE_END  (BOOST_EXPECTED_FUTURE_V1)

#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_EXPECTED_FUTURE_V1, (stl11, inline))
#define BOOST_STL11_ATOMIC_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_EXPECTED_FUTURE_V1, (stl11, inline))
#define BOOST_STL11_FUTURE_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_EXPECTED_FUTURE_V1, (stl11, inline))
#define BOOST_STL11_FUTURE_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_EXPECTED_FUTURE_V1, (stl11, inline))
#define BOOST_STL11_FUTURE_MAP_NO_FUTURE
#define BOOST_STL11_FUTURE_MAP_NO_SHARED_FUTURE
#define BOOST_STL11_FUTURE_MAP_NO_PROMISE
#define BOOST_STL11_THREAD_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_EXPECTED_FUTURE_V1, (stl11, inline))
#define BOOST_STL11_THREAD_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_EXPECTED_FUTURE_V1, (stl11, inline))
#define BOOST_STL11_CHRONO_MAP_NAMESPACE_BEGIN        BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_EXPECTED_FUTURE_V1, (stl11, inline), (chrono))
#define BOOST_STL11_CHRONO_MAP_NAMESPACE_END          BOOST_BINDLIB_NAMESPACE_END  (BOOST_EXPECTED_FUTURE_V1, (stl11, inline), (chrono))
#include BOOST_BINDLIB_INCLUDE_STL11(bindlib, BOOST_EXPECTED_FUTURE_V1_STL11_IMPL, atomic)
#include BOOST_BINDLIB_INCLUDE_STL11(bindlib, BOOST_EXPECTED_FUTURE_V1_STL11_IMPL, future)
#include BOOST_BINDLIB_INCLUDE_STL11(bindlib, BOOST_EXPECTED_FUTURE_V1_STL11_IMPL, thread)
#include BOOST_BINDLIB_INCLUDE_STL11(bindlib, BOOST_EXPECTED_FUTURE_V1_STL11_IMPL, chrono)

#define BOOST_SPINLOCK_MAP_NAMESPACE_BEGIN BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_EXPECTED_FUTURE_V1, (expected_spinlock, inline))
#define BOOST_SPINLOCK_MAP_NAMESPACE_END   BOOST_BINDLIB_NAMESPACE_END(BOOST_EXPECTED_FUTURE_V1, (expected_spinlock, inline))
#include "spinlock.bind.hpp"

// Bind exception_ptr
BOOST_BINDLIB_NAMESPACE_BEGIN(BOOST_EXPECTED_FUTURE_V1, (stl11, inline))
  using BOOST_EXPECTED_FUTURE_V1_STL11_IMPL::exception_ptr;
BOOST_BINDLIB_NAMESPACE_END(BOOST_EXPECTED_FUTURE_V1, (stl11, inline))

BOOST_EXPECTED_FUTURE_V1_NAMESPACE_BEGIN

  BOOST_BINDLIB_DECLARE(BOOST_EXPECTED_FUTURE_V1, "TODO FIXME")  // TODO FIXME
  
  /*! \class basic_future
   * \brief Provides a source of an instance of some type FutureType at some future point
   * 
   * \tparam consuming Whether the basic_future provides consuming semantics (see below).
   * \tparam FutureType The type provided by the future in some time from now. This must match the FutureType concept (see below).
   * 
   * \todo Any basic_future not used expected<T, E> as FutureType
   * \todo future<T&>
   * \todo future<void>
   *
   * Consuming semantics are:
   *  - future.get() move constructs the value, thus destroying the future's copy of it. Non-consuming returns a const lvalue ref.
   *  - promise.get_future() never returns a future more than once. Non-consuming it will return a new future if no future is
   *    currently associated with the promise. Note that racing on promise.get_future() with non-consuming semantics is a bad idea,
   *    it may fatal exit the process.
   * 
   * The FutureType concept is:
   *  - Needs to provide a value() member function returning a reference to the type
   */
  template<bool consuming, class FutureType, class... Continuations> class basic_future;
  template<class BasicFutureType, class PtrType=std::shared_ptr<BasicFutureType>> class basic_future_ref;
  template<class FutureType> class basic_promise;
  namespace detail
  {
    template<class me_type, class other_type> struct future_promise_unlocker
    {
      me_type *me;
      other_type *other;
      decltype(me_type::_lock) *me_lock;
      decltype(other_type::_lock) *other_lock;
      BOOST_SPINLOCK_CONSTEXPR future_promise_unlocker(me_type *f, other_type *p) BOOST_NOEXCEPT : me(f), other(p), me_lock(f ? &f->_lock : nullptr), other_lock(p ? &p->_lock : nullptr) { }
      future_promise_unlocker(future_promise_unlocker &&o) BOOST_NOEXCEPT : me(o.me), other(o.other), me_lock(o.me_lock), other_lock(o.other_lock) { me_lock=nullptr; other_lock=nullptr; }
      future_promise_unlocker(const future_promise_unlocker &) = delete;
      future_promise_unlocker &operator=(const future_promise_unlocker &) = delete;
      future_promise_unlocker &operator=(future_promise_unlocker &&) = delete;
      ~future_promise_unlocker() BOOST_NOEXCEPT
      {
        unlock();
      }
      void unlock() BOOST_NOEXCEPT
      {
        if(me_lock)
        {
          me_lock->unlock();
          me_lock=nullptr;
        }
        if(other_lock)
        {
          other_lock->unlock();
          other_lock=nullptr;
        }
      }
    };
    template<class S, class D=typename S::other_type> future_promise_unlocker<S, D> lock_future_promise(S *me) BOOST_NOEXCEPT
    {
      for(size_t count=0;;count++)
      {
        // Lock myself first
        if(me->_lock.try_lock())
        {
          if(!me->_other || me->_other->_lock.try_lock())
            return future_promise_unlocker<S, D>(me, me->_other);
          me->_lock.unlock();
        }
        if(count>10)
          this_thread::sleep_for(chrono::milliseconds(1));
        else
          this_thread::yield();        
      }      
    }
    template<class value_type, bool consuming> struct get_impl
    {
      auto operator()(value_type &&me) -> typename std::decay<decltype(me.value())>::type
      {
        auto ret(std::move(me.value()));
        return ret;
      }
    };
    template<class value_type> struct get_impl<value_type, false>
    {
      auto operator()(value_type &&me) -> typename std::decay<decltype(me.value())>::type const&
      {
        return me.value();
      }
    };
  }
  template<bool consuming, class T, class E> class basic_future<consuming, expected<T, E>> : protected expected<T, E>
  {
    template<class me_type, class other_type> friend struct detail::future_promise_unlocker;
    template<class S, class D> friend detail::future_promise_unlocker<S, D> detail::lock_future_promise(S *) BOOST_NOEXCEPT;
    friend class basic_promise<basic_future>;
  public:
    typedef basic_future<consuming, expected<T, E>> future_type;
    typedef basic_promise<future_type> promise_type;
    typedef promise_type other_type;
    typedef expected<T, E> value_type;
    static BOOST_CONSTEXPR_OR_CONST bool get_consumes=consuming;
    typedef T gettable_type;
    typedef E exception_type;
  private:
    spinlock<bool> _lock;
    mutable spinlock<bool> _ready; // _ready is INVERTED
    atomic<bool> _abandoned;
    promise_type *_other;
    void _detach() BOOST_NOEXCEPT
    {
      if(_other)
      {
        _other->_other=nullptr;
        _other=nullptr;
      }
    }
    inline void _abandon() BOOST_NOEXCEPT
    {
      _abandoned=true;
      _detach();
    }
    template<class U> void _set(U &&v)
    {
      expected<T, E>::operator=(std::forward<U>(v));
      _ready.unlock();
    }
      
    BOOST_RELAXED_CONSTEXPR basic_future(promise_type *p, bool ready, expected<T, E> &&o) BOOST_NOEXCEPT_IF((std::is_nothrow_default_constructible<expected<T, E>>::value)) : _abandoned(false), _other(p), expected<T, E>(std::move(o)) { if(!ready) _ready.lock(); }
  public:
    BOOST_RELAXED_CONSTEXPR basic_future() BOOST_NOEXCEPT_IF((std::is_nothrow_default_constructible<expected<T, E>>::value)) : _abandoned(false), _other(nullptr) { _ready.lock(); }
    basic_future(basic_future &&o) BOOST_NOEXCEPT_IF((std::is_nothrow_move_assignable<expected<T, E>>::value && std::is_nothrow_default_constructible<expected<T, E>>::value)) : basic_future() { this->operator=(std::move(o)); }
    basic_future(const basic_future &o) = delete;
    ~basic_future() BOOST_NOEXCEPT_IF((std::is_nothrow_destructible<expected<T, E>>::value))
    {
      auto lock=detail::lock_future_promise(this);
      _detach();
    }
    basic_future &operator=(basic_future &&o) BOOST_NOEXCEPT_IF((std::is_nothrow_move_assignable<expected<T, E>>::value))
    {
      // First lock myself and detach myself from any promise of mine
      auto lock1=detail::lock_future_promise(this);
      _detach();
      // Lock source and his promise
      auto lock2=detail::lock_future_promise(&o);
      _ready.unlock();
      if(!o.is_ready()) _ready.lock();
      _abandoned.store(o._abandoned.load(memory_order_relaxed), memory_order_relaxed);
      _other=o._other;
      expected<T, E>::operator=(std::move(o));
      _other->_other=this;
      if(o.is_ready()) _ready.lock();
      o._abandoned.store(false, memory_order_relaxed);
      o._other=nullptr;
      return *this;
    }
    basic_future &operator=(const basic_future &o) = delete;
    inline basic_future_ref<basic_future> share();
    decltype(detail::get_impl<value_type, consuming>()(value_type())) get()
    {
      wait();
      // If we are consuming, destroy the relationship with my promise now
      if(consuming)
      {
        auto lock=detail::lock_future_promise(this);
        _detach();
      }
      return detail::get_impl<value_type, consuming>()(std::move(*this));
    }
    BOOST_SPINLOCK_CONSTEXPR bool valid() const BOOST_NOEXCEPT { return _other!=nullptr; }
    BOOST_SPINLOCK_CONSTEXPR bool is_consuming() const BOOST_NOEXCEPT { return consuming; }
    BOOST_SPINLOCK_CONSTEXPR bool is_ready() const BOOST_NOEXCEPT { return !is_lockable_locked(_ready); }
    BOOST_SPINLOCK_CONSTEXPR bool is_abandoned() const BOOST_NOEXCEPT { return _abandoned; }
    BOOST_SPINLOCK_CONSTEXPR bool has_exception() const BOOST_NOEXCEPT { return is_ready() && !expected<T, E>::valid(); }
    BOOST_SPINLOCK_CONSTEXPR bool has_value() const BOOST_NOEXCEPT { return is_ready() && expected<T, E>::valid(); }
    void wait() const
    {
      if(!valid()) throw future_error(future_errc::no_state);
      if(is_abandoned()) throw future_error(future_errc::broken_promise);
      // TODO: Use a permit object here instead of a spinlock
      if(!is_ready())
      {
        _ready.lock();
        _ready.unlock();
      }
    }
    template<class Rep, class Period>
    future_status wait_for(const chrono::duration<Rep,Period> &timeout_duration) const;
    template<class Clock, class Duration>
    future_status wait_until(const chrono::time_point<Clock,Duration> &timeout_time) const;
  };
  
  /*! \class basic_future_ref
   * \brief A reference to a basic_future managed by a PtrType. Typically used to implement shared_future.
   */
  template<class BasicFutureType> class basic_future_ref<BasicFutureType, std::shared_ptr<BasicFutureType>>
  {
  public:
    typedef BasicFutureType future_type;
    typedef typename BasicFutureType::promise_type promise_type;
    typedef promise_type other_type;
    typedef typename BasicFutureType::value_type value_type;
    static BOOST_CONSTEXPR_OR_CONST bool get_consumes=BasicFutureType::get_consumes;
    typedef typename BasicFutureType::gettable_type gettable_type;
    typedef typename BasicFutureType::exception_type exception_type;
    typedef std::shared_ptr<BasicFutureType> managing_type;
  private:
    managing_type _p;
    BasicFutureType *__p()
    {
      if(!_p)
        throw future_error(future_errc::no_state);
      return _p.get();
    }
    BOOST_SPINLOCK_CONSTEXPR BasicFutureType *__p() const
    {
      if(!_p)
        throw future_error(future_errc::no_state);
      return _p.get();
    }
  public:    
    BOOST_SPINLOCK_CONSTEXPR basic_future_ref() BOOST_NOEXCEPT_IF((std::is_nothrow_default_constructible<managing_type>::value)) { }
    BOOST_SPINLOCK_CONSTEXPR basic_future_ref(BasicFutureType &&o) : _p(std::make_shared<BasicFutureType>(std::move(o))) { }
#if defined(_MSC_VER) && _MSC_VER >= 1900  // >= VS2015
    basic_future_ref(basic_future_ref &&) = default;
#endif
    basic_future_ref(const basic_future_ref &) = default;
    decltype(detail::get_impl<value_type, get_consumes>()(value_type())) get()
    {
      typedef decltype(detail::get_impl<value_type, get_consumes>()(value_type())) rettype;
      return std::forward<rettype>(__p()->get());
    }
    BOOST_SPINLOCK_CONSTEXPR bool valid() const BOOST_NOEXCEPT { return _p && __p()->valid(); }
    BOOST_SPINLOCK_CONSTEXPR bool is_consuming() const BOOST_NOEXCEPT { return __p()->is_consuming(); }
    BOOST_SPINLOCK_CONSTEXPR bool is_ready() const BOOST_NOEXCEPT { return __p()->is_ready(); }
    BOOST_SPINLOCK_CONSTEXPR bool is_abandoned() const BOOST_NOEXCEPT { return __p()->is_abandoned(); }
    BOOST_SPINLOCK_CONSTEXPR bool has_exception() const BOOST_NOEXCEPT { return __p()->has_exception(); }
    BOOST_SPINLOCK_CONSTEXPR bool has_value() const BOOST_NOEXCEPT { return __p()->has_value(); }
    void wait() const { return __p()->wait(); }
    template<class Rep, class Period>
    future_status wait_for(const chrono::duration<Rep,Period> &timeout_duration) const;
    template<class Clock, class Duration>
    future_status wait_until(const chrono::time_point<Clock,Duration> &timeout_time) const;
  };
  template<bool consuming, class T, class E> inline basic_future_ref<basic_future<consuming, expected<T, E>>> basic_future<consuming, expected<T, E>>::share()
  {
    return basic_future_ref<basic_future<consuming, expected<T, E>>>(std::move(*this));
  }

  /*! \class basic_promise
   * \brief Provides a sink of an instance of some type FutureType
   */
  template<bool consuming, class T, class E> class basic_promise<basic_future<consuming, expected<T, E>>> : protected expected<T, E>
  {
    template<class me_type, class other_type> friend struct detail::future_promise_unlocker;
    template<class S, class D> friend detail::future_promise_unlocker<S, D> detail::lock_future_promise(S *) BOOST_NOEXCEPT;
    friend class basic_future<consuming, expected<T, E>>;
  public:
    typedef basic_future<consuming, expected<T, E>> future_type;
    typedef basic_promise<future_type> promise_type;
    typedef future_type other_type;
  private:
    spinlock<bool> _lock;
    atomic<bool> _ready, _retrieved;
    future_type *_other;
    void _abandon() BOOST_NOEXCEPT
    {
      if(_other)
      {
         if(!_other->is_ready())
           _other->_abandon();
         else
           _other->_detach();
      }
    }
    template<class U> void _set(U &&v)
    {
      if(_ready)
        throw future_error(future_errc::promise_already_satisfied);
      auto lock=detail::lock_future_promise(this);
      // If no future exists yet, set myself to the value and we'll set the future on creation
      if(!_other)
        expected<T, E>::operator=(std::forward<U>(v));
      else
        _other->_set(std::forward<U>(v));
      _ready=true;
    }
  public:
    BOOST_SPINLOCK_CONSTEXPR basic_promise() BOOST_NOEXCEPT_IF((std::is_nothrow_default_constructible<expected<T, E>>::value)) : _ready(false), _retrieved(false), _other(nullptr)
    {
      static_assert(!std::uses_allocator<basic_promise, std::allocator<T>>::value, "Non-allocating future-promise cannot make use of allocators");
      static_assert(!std::uses_allocator<basic_promise, std::allocator<E>>::value, "Non-allocating future-promise cannot make use of allocators");
    }
    basic_promise(basic_promise &&o) BOOST_NOEXCEPT_IF((std::is_nothrow_move_assignable<expected<T, E>>::value)) : basic_promise() { this->operator=(std::move(o)); }
    basic_promise(const basic_promise &) = delete;
    ~basic_promise()
    {
      auto lock=detail::lock_future_promise(this);
      _abandon();
    }
    basic_promise &operator=(basic_promise &&o) BOOST_NOEXCEPT_IF((std::is_nothrow_move_assignable<expected<T, E>>::value))
    {
      // First lock myself and detach/abandon any future of mine
      auto lock1=detail::lock_future_promise(this);
      _abandon();
      // Lock source and his future
      auto lock2=detail::lock_future_promise(&o);
      _ready.store(o._ready.load(memory_order_relaxed), memory_order_relaxed);
      _retrieved.store(o._retrieved.load(memory_order_relaxed), memory_order_relaxed);
      _other=o._other;
      expected<T, E>::operator=(std::move(o));
      _other->_other=this;
      o._ready.store(false, memory_order_relaxed);
      o._retrieved.store(false, memory_order_relaxed);
      o._other=nullptr;
      return *this;
    }
    basic_promise &operator=(const basic_promise &) = delete;
    BOOST_SPINLOCK_CONSTEXPR bool is_ready() const BOOST_NOEXCEPT { return _ready; }
    BOOST_SPINLOCK_CONSTEXPR bool is_future_retrieved() const BOOST_NOEXCEPT { return _retrieved || _other; }
    void swap(basic_promise &o) BOOST_NOEXCEPT;
    future_type get_future() BOOST_NOEXCEPT_IF((!consuming && std::is_nothrow_default_constructible<future_type>::value && std::is_nothrow_move_constructible<future_type>::value))
    {
      // If I am consuming semantics, never allow a new future after the first is fetched
      if(consuming && _retrieved)
        throw future_error(future_errc::future_already_retrieved);
      auto lock1=detail::lock_future_promise(this);
      // If I am non-consuming semantics, there can be only one future associated with this promise at a time
      // As basic_shared_future<> will never fetch a future twice, this will terminate the process if get_future() is noexcept
      if((consuming && _retrieved) || _other)
        throw future_error(future_errc::future_already_retrieved);      
      bool ready=_ready.load(memory_order_relaxed);
      if(consuming)
        _retrieved.store(true, memory_order_relaxed);
      if(ready)
      {
        future_type ret(this, ready, std::move(*this));
        _other=&ret;
        // The move constructor will try to lock me, so preempt
        lock1.unlock();
        return ret;
      }
      else
      {
        future_type ret(this, ready, expected<T, E>());
        _other=&ret;
        // The move constructor will try to lock me, so preempt
        lock1.unlock();
        return ret;
      }
    }
    void set_value(const T &v)
    {
      _set(v);
    }
    void set_value(T &&v)
    {
      _set(std::move(v));
    }
    // set_value_at_thread_exit() not possible with this design
    void set_exception(const E &v)
    {
      _set(make_unexpected(v));
    }
    void set_exception(E &&v)
    {
      _set(make_unexpected(std::move(v)));
    }
    // set_exception_at_thread_exit() not possible with this design
  };
  template<class T, class E=exception_ptr> using future = basic_future<true, expected<T, E>>;
  template<class T, class E=exception_ptr> using promise = basic_promise<future<T, E>>;
  template<class T, class E=exception_ptr> using shared_future = basic_future_ref<basic_future<false, expected<T, E>>>;

BOOST_EXPECTED_FUTURE_V1_NAMESPACE_END

namespace std
{
  template<bool consuming, class ContainerType, class... Continuations> void swap(BOOST_EXPECTED_FUTURE_V1_NAMESPACE::basic_future<consuming, ContainerType, Continuations...> &a, BOOST_EXPECTED_FUTURE_V1_NAMESPACE::basic_future<consuming, ContainerType, Continuations...> &b) BOOST_NOEXCEPT
  {
    a.swap(b);
  }
  template<class FutureType> void swap(BOOST_EXPECTED_FUTURE_V1_NAMESPACE::basic_promise<FutureType> &a, BOOST_EXPECTED_FUTURE_V1_NAMESPACE::basic_promise<FutureType> &b) BOOST_NOEXCEPT
  {
    a.swap(b);
  }
}

#endif

// Do local namespace binding
#ifdef BOOST_EXPECTED_FUTURE_V1_MAP_NAMESPACE_BEGIN

BOOST_EXPECTED_FUTURE_V1_MAP_NAMESPACE_BEGIN
template<class T, class... Args> using basic_promise = BOOST_EXPECTED_FUTURE_V1_NAMESPACE::basic_promise<T, Args...>;
template<class T, class E> class basic_shared_future = BOOST_EXPECTED_FUTURE_V1_NAMESPACE::basic_shared_future<T, E>;
template<class T, class E> class basic_future = BOOST_EXPECTED_FUTURE_V1_NAMESPACE::basic_future<T, E>;
BOOST_EXPECTED_FUTURE_V1_MAP_NAMESPACE_END

#endif
