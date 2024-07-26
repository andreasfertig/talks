#include <coroutine>

struct suspend_never {
  constexpr bool await_ready() const noexcept
  {
    return true;
  }

  constexpr void
  await_suspend(std::coroutine_handle<>) const noexcept
  {}

  constexpr void await_resume() const noexcept {}
};

int main() {}
