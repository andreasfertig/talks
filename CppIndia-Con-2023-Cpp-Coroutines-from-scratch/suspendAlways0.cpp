#include <coroutine>

struct suspend_always {
  constexpr bool await_ready() const noexcept
  {
    return false;
  }

  constexpr void
  await_suspend(std::coroutine_handle<>) const noexcept
  {}

  constexpr void await_resume() const noexcept {}
};