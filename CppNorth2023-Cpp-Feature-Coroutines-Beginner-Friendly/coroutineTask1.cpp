#include <coroutine>
#include <cstdio>
#include <exception>  // std::terminate
#include <iostream>
#include <list>
#include <new>
#include <utility>

struct Task {
  struct promise_type {
    Task               get_return_object() noexcept { return {}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void               return_void() noexcept {}
    void               unhandled_exception() noexcept {}
  };
};

struct Scheduler {
  std::list<std::coroutine_handle<>> _tasks{};

  void suspend(std::coroutine_handle<> coro) { _tasks.push_back(coro); }

  bool schedule()
  {
    auto task = _tasks.front();
    _tasks.pop_front();

    if(not task.done()) { task.resume(); }

    return not _tasks.empty();
  }
};

static Scheduler gScheduler{};

struct suspend {
  auto operator co_await()
  {
    struct awaiter : std::suspend_always {
      void await_suspend(std::coroutine_handle<> coro) const noexcept { gScheduler.suspend(coro); }
    };

    return awaiter{};
  }
};

Task taskA()
{
  std::cout << "Hello, from task A\n";

  co_await suspend{};

  std::cout << "a is back doing work\n";

  co_await suspend{};

  std::cout << "a is back doing more work\n";
}

Task taskB()
{
  std::cout << "Hello, from task B\n";

  co_await suspend{};

  std::cout << "b is back doing work\n";

  co_await suspend{};

  std::cout << "b is back doing more work\n";
}

int main()
{
  taskA();
  taskB();

  while(gScheduler.schedule()) {}
}