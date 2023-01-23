#include <coroutine>
#include <cstdio>
#include <exception>  // std::terminate
#include <iostream>
#include <list>
#include <new>
#include <utility>

struct Task {
  struct promise_type {
    Task               get_return_object() { return {}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void               unhandled_exception() {}
    void               return_void() noexcept {}
  };
};

struct Scheduler {
  std::list<std::coroutine_handle<>> _tasks{};

  bool schedule()
  {
    auto task = _tasks.front();
    _tasks.pop_front();

    if(not task.done()) { task.resume(); }

    return not _tasks.empty();
  }

  auto suspend()
  {
    struct awaiter : std::suspend_always {
      Scheduler& _sched;

      explicit awaiter(Scheduler& sched) : _sched{sched} {}
      void await_suspend(std::coroutine_handle<> coro) const noexcept { _sched._tasks.push_back(coro); }
    };

    return awaiter{*this};
  }
};

Task taskA(Scheduler& sched)
{
  std::cout << "Hello, from task A\n";

  co_await sched.suspend();

  std::cout << "a is back doing work\n";

  co_await sched.suspend();

  std::cout << "a is back doing more work\n";
}

Task taskB(Scheduler& sched)
{
  std::cout << "Hello, from task B\n";

  co_await sched.suspend();

  std::cout << "b is back doing work\n";

  co_await sched.suspend();

  std::cout << "b is back doing more work\n";
}

int main()
{
  Scheduler scheduler{};

  taskA(scheduler);
  taskB(scheduler);

  while(scheduler.schedule()) {}
}
