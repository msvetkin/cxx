// SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
// SPDX-License-Identifier: MIT

#pragma once

#include "cxx/core/coroutine.hpp"

#include <exception>
#include <tl/expected.hpp>
#include <type_traits>
#include <utility>

namespace cxx::core {

template <typename T, typename Error>
using expected = tl::expected<T, Error>;

template <typename Error>
using unexpected = tl::unexpected<Error>;

template <typename Error>
unexpected<std::decay_t<Error>> make_unexpected(Error &&error) {
  return tl::make_unexpected(std::forward<Error>(error));
}

namespace _expected {

template <typename T>
struct isUnexpected : std::false_type {};

template <typename T>
struct isUnexpected<unexpected<T>> : std::true_type {};

template <typename T, typename Error>
struct Promise;

template <typename T, typename Error>
class PromiseReturn {
 public:
  PromiseReturn(Promise<T, Error> &promise) noexcept
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

  operator expected<T, Error>() noexcept {
    return std::move(storage_);
  }

 private:
  expected<T, Error> storage_;
  Promise<T, Error> *promise_{};
};

template <typename T, typename Error>
struct Promise {
  Promise() = default;
  Promise(const Promise &) = delete;

  PromiseReturn<T, Error> get_return_object() noexcept {
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
    if constexpr (isUnexpected<U>::value) {
      *value_ = static_cast<U &&>(u);
    } else {
      value_->emplace(static_cast<U &&>(u));
    }
  }

  void unhandled_exception() {
    std::terminate();
  }

  expected<T, Error> *value_{};
};

template <typename T, typename Error>
class Awaitable {
 public:
  explicit Awaitable(expected<T, Error> expected)
      : expected_(std::move(expected)) {
  }

  bool await_ready() const noexcept {
    return expected_.has_value();
  }

  T await_resume() {
    return std::move(expected_).value();
  }

  template <typename U>
  void await_suspend(std::coroutine_handle<Promise<U, Error>> h) const {
    *h.promise().value_ = unexpected<Error>(std::move(expected_.error()));
    h.destroy();
  }

 private:
  expected<T, Error> expected_;
};

}  // namespace _expected

}  // namespace cxx::core

template <typename T, typename Error, typename... Args>
struct std::coroutine_traits<cxx::core::expected<T, Error>, Args...> {
  using promise_type = cxx::core::_expected::Promise<T, Error>;
};

template <typename T, typename Error>
auto operator co_await(cxx::core::expected<T, Error> expected) noexcept {
  return cxx::core::_expected::Awaitable{std::move(expected)};
}
