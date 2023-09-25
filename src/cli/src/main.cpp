// SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
// SPDX-License-Identifier: MIT

#include "cxx/http/http.hpp"

#include <fmt/core.h>

int main(int /*argc*/, char * /*argv*/ []) {
  fmt::print("cxx version: {}\n", cxx::http::version());
  return 0;
}
