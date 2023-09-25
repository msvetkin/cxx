# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

add_cxx_module(http)

target_sources(${cxx_module_target} PRIVATE
  include/cxx/http/http.hpp
  src/http.cpp)
