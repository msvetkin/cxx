// SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
// SPDX-License-Identifier: MIT

#include "cxx/core/optional.hpp"

#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <vector>

TEST_CASE("header sanity check") {
}

TEST_CASE("co_await, nullopt") {
  std::vector<int> steps;

  auto getValue = [&steps]() -> std::optional<int> {
    steps.push_back(1);
    std::optional<int> v;
    steps.push_back(2);
    co_await v;
    steps.push_back(3);
    co_return 10;
  };

  const auto v = getValue();
  REQUIRE(std::same_as<const std::optional<int>, decltype(v)>);
  REQUIRE(v.has_value() == false);
  REQUIRE(steps == std::vector{1, 2});
}

TEST_CASE("co_await, value") {
  std::vector<int> steps;

  auto getValue = [&steps]() -> std::optional<int> {
    steps.push_back(1);
    std::optional<int> v{10};
    steps.push_back(2);
    const auto value = co_await v;
    steps.push_back(3);
    co_return value + 1;
  };

  const auto v = getValue();
  REQUIRE(std::same_as<const std::optional<int>, decltype(v)>);
  REQUIRE(v.has_value());
  REQUIRE(*v == 11);
  REQUIRE(steps == std::vector{1, 2, 3});
}
