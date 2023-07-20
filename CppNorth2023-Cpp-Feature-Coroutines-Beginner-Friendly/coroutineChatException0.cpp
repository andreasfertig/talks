#include <coroutine>
#include <cstdio>
#include <exception>  // std::terminate
#include <iostream>
#include <list>
#include <new>
#include <string>
#include <utility>

using namespace std::literals;

struct Chat {
  struct promise_type {
    std::string _msgOut{}, _msgIn{};

    void                unhandled_exception() { throw; }  // #A Re-throw
    Chat                get_return_object() { return Chat{this}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always yield_value(std::string msg) noexcept
    {
      _msgOut = std::move(msg);
      return {};
    }

    auto await_transform(std::string) noexcept
    {
      struct awaiter {
        promise_type&  pt;
        constexpr bool await_ready() const noexcept { return true; }
        std::string    await_resume() const noexcept { return std::move(pt._msgIn); }
        void           await_suspend(std::coroutine_handle<>) const noexcept {}
      };

      return awaiter{*this};
    }

    void                return_value(std::string msg) noexcept { _msgOut = std::move(msg); }
    std::suspend_always final_suspend() noexcept { return {}; }
  };

  using Handle = std::coroutine_handle<promise_type>;  // #A Shortcut for the handle type
  Handle mCoroHdl{};                                   // #B

  explicit Chat(promise_type* p) : mCoroHdl{Handle::from_promise(*p)} {}  // #C Get the handle form the promise
  Chat(Chat&& rhs) : mCoroHdl{std::exchange(rhs.mCoroHdl, nullptr)} {}    // #D Move only!

  ~Chat()  // #E Care taking, destroying the handle if needed
  {
    if(mCoroHdl) { mCoroHdl.destroy(); }
  }

  std::string listen()  // #F Active the coroutine and wait for data.
  {
    if(not mCoroHdl.done()) { mCoroHdl.resume(); }
    return std::move(mCoroHdl.promise()._msgOut);
  }

  void answer(std::string msg)  // #G Send data to the coroutine and activate it.
  {
    mCoroHdl.promise()._msgIn = std::move(msg);
    if(not mCoroHdl.done()) { mCoroHdl.resume(); }
  }
};

Chat Fun()
{
  throw int{42};  // #A Simulating an exception
}

int main()
{
  try {  // #B Hence it is good to catch an exception
    Chat chat = Fun();

    std::cout << chat.listen();

  } catch(std::exception& ex) {
    std::cout << ex.what();
  }
}