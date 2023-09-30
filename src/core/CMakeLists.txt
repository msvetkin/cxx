# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

add_cxx_module(core INTERFACE)

target_sources(${cxx_module_target}
  INTERFACE
    include/cxx/core/coroutine.hpp
    include/cxx/core/optional.hpp
)
