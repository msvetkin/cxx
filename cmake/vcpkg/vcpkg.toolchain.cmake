# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.25)

get_property(IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE)

if (IN_TRY_COMPILE)
  return()
endif()

unset(IN_TRY_COMPILE)

if (NOT DEFINED CACHE{VCPKG_TARGET_TRIPLET} AND
    DEFINED PRESET_NAME_TO_VCPKG_TARGET_TRIPLET)
  include(${CMAKE_CURRENT_LIST_DIR}/vcpkg_autodetect_target_triplet.cmake)
  vcpkg_autodetect_target_target_triplet()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/vcpkg_bootstrap.cmake)

vcpkg_bootstrap(
  CACHE_DIR_NAME cxx
  REPO https://github.com/microsoft/vcpkg.git
  REF 9edb1b8e590cc086563301d735cae4b6e732d2d2 # release 2023.08.09
)

file(SHA512 "${CMAKE_SOURCE_DIR}/vcpkg.json" vcpkg_manifest_hash)

if (_VCPKG_MANIFEST_HASH STREQUAL vcpkg_manifest_hash)
  # cmake-lint: disable=C0103
  set(VCPKG_MANIFEST_INSTALL OFF CACHE INTERNAL "")
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
      "${CMAKE_SOURCE_DIR}/vcpkg.json")
else()
  # cmake-lint: disable=C0103
  set(VCPKG_MANIFEST_INSTALL ON CACHE INTERNAL "")
endif()

set(_VCPKG_MANIFEST_HASH "${vcpkg_manifest_hash}"
    CACHE INTERNAL "Hash of vcpkg manifest file")

include($CACHE{_VCPKG_TOOLCHAIN_FILE})
