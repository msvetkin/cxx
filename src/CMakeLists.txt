# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25)

project(
  cxx
  VERSION 0.0.1
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# include(cxx-install-targets)
include(add_cxx_module)
include(add_cxx_executable)

add_subdirectory(core)
add_subdirectory(http)
add_subdirectory(cli)
