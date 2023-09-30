// SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
// SPDX-License-Identifier: MIT

#include "cxx/core/expected.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <concepts>
#include <vector>

TEST_CASE("header sanity check") {
}

TEST_CASE("co_await, error") {
  std::vector<int> steps;

  auto foo = []() -> std::expected<int, std::string> {
    return std::unexpected("something went wrong");
  };

  auto getValue = [&steps, foo]() -> std::expected<int, std::string> {
    steps.push_back(1);
    const auto result = foo();
    steps.push_back(2);
    co_await result;
    steps.push_back(3);
  };

  const auto v = getValue();
  REQUIRE(std::same_as<const std::expected<int, std::string>, decltype(v)>);
  REQUIRE(v.has_value() == false);
  REQUIRE(v.error() == "something went wrong");
  REQUIRE(steps == std::vector{1, 2});
}

TEST_CASE("co_await, value") {
  std::vector<int> steps;

  auto foo = []() -> std::expected<int, std::string> {
    return 10;
  };

  auto getValue = [&steps, foo]() -> std::expected<int, std::string> {
    steps.push_back(1);
    const auto result = foo();
    steps.push_back(2);
    const auto value = co_await result;
    steps.push_back(3);
    co_return value + 1;
  };

  const auto v = getValue();
  REQUIRE(std::same_as<const std::expected<int, std::string>, decltype(v)>);
  REQUIRE(v.has_value());
  REQUIRE(*v == 11);
  REQUIRE(steps == std::vector{1, 2, 3});
}

TEST_CASE("co_await, nested") {
  const auto &&[id, error] = GENERATE(table<int, std::string>({
      std::make_tuple(1, ""),
      std::make_tuple(10, "failed to create request"),
  }));

  auto makeRequest = [](const int id) -> std::expected<int, std::string> {
    return id;
  };

  auto sendRequest =
      [makeRequest](const int id) -> std::expected<int, std::string> {
    const auto request = co_await makeRequest(id);
    if (request == 10) {
      co_return std::unexpected{"failed to create request"};
    }

    co_return request + 10;
  };

  auto getValue =
      [sendRequest](const int id) -> std::expected<int, std::string> {
    co_return co_await sendRequest(id);
  };

  const auto v = getValue(id);
  REQUIRE(std::same_as<const std::expected<int, std::string>, decltype(v)>);

  if (error.empty()) {
    REQUIRE(v.has_value());
    REQUIRE(*v == id + 10);
  } else {
    REQUIRE(v.has_value() == false);
    REQUIRE(v.error() == error);
  }
}
