#include <coroutine>
#include <cstdio>
#include <exception>  // std::terminate
#include <iostream>
#include <new>
#include <utility>
#include <vector>

struct Generator {
  struct promise_type {
    int _val{};

    Generator           get_return_object() { return Generator{this}; }
    std::suspend_never  initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    std::suspend_always yield_value(int v)
    {
      _val = v;
      return {};
    }

    void unhandled_exception() {}

    void return_void() noexcept {}
  };

  // struct Generator {
  using Handle = std::coroutine_handle<promise_type>;
  Handle mCoroHdl{};

  explicit Generator(promise_type* p) : mCoroHdl{Handle::from_promise(*p)} {}

  Generator(Generator&& rhs) : mCoroHdl{std::exchange(rhs.mCoroHdl, nullptr)} {}

  ~Generator()
  {
    if(mCoroHdl) { mCoroHdl.destroy(); }
  }

  int value() const { return mCoroHdl.promise()._val; }

  bool finished() const { return mCoroHdl.done(); }

  void resume()
  {
    if(not finished()) { mCoroHdl.resume(); }
  }

  struct sentinel {};

  struct iterator {
    Handle mCoroHdl{};

    bool operator==(sentinel) const { return mCoroHdl.done(); }

    iterator& operator++()
    {
      mCoroHdl.resume();
      return *this;
    }

    const int operator*() const { return mCoroHdl.promise()._val; }
  };

  // struct Generator {
  // ...
  iterator begin() { return {mCoroHdl}; }
  sentinel end() { return {}; }
  // };
};

Generator interleaved(std::vector<int> a, std::vector<int> b)
{
  auto lamb = [](std::vector<int>& v) -> Generator {
    for(const auto& e : v) { co_yield e; }
  };

  auto x = lamb(a);
  auto y = lamb(b);

  while(not x.finished() or not y.finished()) {
    if(not x.finished()) {
      co_yield x.value();
      x.resume();
    }

    if(not y.finished()) {
      co_yield y.value();
      y.resume();
    }
  }
}

void UseIterator()
{
  std::vector a{2, 4, 6, 8};
  std::vector b{3, 5, 7, 9};

  Generator g{interleaved(std::move(a), std::move(b))};

  for(const auto& e : g) { std::cout << e << '\n'; }
}

int main()
{
  std::vector a{2, 4, 6, 8};
  std::vector b{3, 5, 7, 9};

  Generator g{interleaved(std::move(a), std::move(b))};

  while(not g.finished()) {
    std::cout << g.value() << '\n';

    g.resume();
  }
}
