# C++20 coroutines cheat sheet

See [Coroutine support](https://en.cppreference.com/w/cpp/coroutine) at cppreference.com for more.

## Library

```cpp
#include <coroutine>
// coroutine_traits
// coroutine_handle
// noop_coroutine
// noop_coroutine_promise
// noop_coroutine_handle
// suspend_never - awaitable
// suspend_always - awaitable
```

## Concepts

### `promise`

```cpp
template <class Value> struct promise {
  coroutine<Value> get_return_object();

  static awaitable<> initial_suspend();
  static awaitable<> final_suspend() noexcept;

  void unhandled_exception();

  // optionally either
  void return_value(Value &&v);
  // or
  void return_void();

  // optionally
  awaitable<> yield_value(Value &&v);

  // optionally
  awaitable<> await_transform(AwaitTarget t);
};
```

### `coroutine`

```cpp
template <class Value> struct coroutine {
  using promise_type = promise<Value>;

  ~coroutine();

  coroutine(coroutine &&that);
  coroutine(std::coroutine_handle<promise<Value>> h);
};
```

### `awaitable`

```cpp
template <class Value>
struct awaitable {
  bool await_ready();

  Value await_resume();

  // either
  template <class Outer>
  void await_suspend(std::coroutine_handle<promise<Outer>> &h);
  // or
  template <class Outer>
  bool await_suspend(std::coroutine_handle<promise<Outer>> &h);
  // or
  template <class Outer, class Other>
  std::coroutine_handle<promise<Other>>
  await_suspend(std::coroutine_handle<promise<Outer>> &h);
};
```

### `operator co_await`

```cpp
// optionally
awaitable<> operator co_await(AwaitTarget value);
```

## Translation

### Coroutine body

```cpp
  promise_type __promise;

  try {
    co_await __promise.initial_suspend();

    // function body

  } catch ( ... ) {
    if (!initial_await_resume_called)
      throw;

    __promise.unhandled_exception();
  }

__final_suspend:
  co_await __promise.final_suspend();
```

### `auto result = co_await expr`

```cpp
// await_transform and operator co_await calls are optional
auto &&__expr = operator co_await(__promise.await_transform(expr));

if (!__expr.await_ready()) {
  __expr.await_suspend(__coroutine_handle);
  // suspend/resume point
}

auto result = __expr.await_resume();
```

### `co_yield expr`

```cpp
co_await __promise.yield_value(expr)
```

### `co_return expr`

```cpp
__promise.return_value(expr);
goto __final_suspend;
```

### `co_return`

```cpp
__promise.return_void();
goto __final_suspend;
```
