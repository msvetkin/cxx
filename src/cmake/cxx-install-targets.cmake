# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

include_guard(GLOBAL)

include(GNUInstallDirs)

add_library(cxx INTERFACE)
add_library(cxx::cxx ALIAS cxx)
install(TARGETS cxx EXPORT cxx-targets)

export(EXPORT cxx-targets NAMESPACE cxx::)
configure_file("cmake/cxx-config.cmake" "." COPYONLY)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  cxx-config-version.cmake COMPATIBILITY SameMajorVersion
)

install(
  EXPORT cxx-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cxx
  NAMESPACE cxx::
)

install(
  FILES cmake/cxx-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cxx-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cxx
)

