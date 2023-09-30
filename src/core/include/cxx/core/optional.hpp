// SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
// SPDX-License-Identifier: MIT

#pragma once

#include "cxx/core/coroutine.hpp"

#include <exception>
#include <optional>

namespace cxx::core {

namespace _optional {

template <typename T>
struct Promise;

template <typename T>
class PromiseReturn {
 public:
  PromiseReturn(Promise<T> &promise) noexcept
      : promise_(&promise) {
    promise_->value_ = &storage_;
  }

  PromiseReturn(PromiseReturn &&that) noexcept
      : PromiseReturn{*that.promise_} {
  }
  PromiseReturn &operator=(PromiseReturn &&) = delete;

  PromiseReturn(const PromiseReturn &) = delete;
  PromiseReturn &operator=(const PromiseReturn &) = delete;

  ~PromiseReturn() = default;

  operator std::optional<T>() & noexcept {
    return std::move(storage_);
  }

 private:
  std::optional<T> storage_;
  Promise<T> *promise_{};
};

template <typename T>
struct Promise {
  Promise() = default;
  Promise(const Promise &) = delete;

  PromiseReturn<T> get_return_object() noexcept {
    return *this;
  }

  std::suspend_never initial_suspend() const noexcept {
    return {};
  }

  std::suspend_never final_suspend() const noexcept {
    return {};
  }

  template <typename U = T>
  void return_value(U &&u) {
    *value_ = static_cast<U &&>(u);
  }

  void unhandled_exception() {
    std::terminate();
  }

  std::optional<T> *value_{};
};

template <typename T>
struct Awaitable {
  std::optional<T> optional_;

  bool await_ready() const noexcept {
    return optional_.has_value();
  }

  T await_resume() {
    return std::move(optional_).value();
  }

  template <typename U>
  void await_suspend(std::coroutine_handle<Promise<U>> h) const {
    h.destroy();
  }
};

}  // namespace _optional

}  // namespace cxx::core

template <typename T, typename... Args>
struct std::coroutine_traits<std::optional<T>, Args...> {
  using promise_type = cxx::core::_optional::Promise<T>;
};

template <typename T>
cxx::core::_optional::Awaitable<T> operator co_await(
    std::optional<T> o) noexcept {
  return {std::move(o)};
}
