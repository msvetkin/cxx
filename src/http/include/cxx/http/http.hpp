// SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
// SPDX-License-Identifier: MIT

#pragma once

#include "cxx/http/export.hpp"

#include <string>

namespace cxx::http {

[[nodiscard]] CXX_HTTP_EXPORT std::string version() noexcept;

} // namespace cxx::http
