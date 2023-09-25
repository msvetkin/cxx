# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

add_cxx_executable(cli)

target_sources(${cxx_executable_target} PRIVATE src/main.cpp)

target_link_libraries(${cxx_executable_target}
  PRIVATE
    cxx::http
)
