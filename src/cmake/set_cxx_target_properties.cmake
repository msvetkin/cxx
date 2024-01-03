# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

include_guard(GLOBAL)

find_package(fmt CONFIG REQUIRED GLOBAL)
find_package(Microsoft.GSL CONFIG REQUIRED GLOBAL)
find_package(range-v3 CONFIG REQUIRED GLOBAL)
find_package(tl-expected CONFIG REQUIRED GLOBAL)

# sets default target properties
function(set_cxx_target_properties target type)
  target_compile_features(${target} ${type} cxx_std_20)
  set_target_properties(${target}
    PROPERTIES
      CXX_STANDARD_REQUIRED ON
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
  )

  if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    target_compile_options(${target} ${type}
      $<$<CXX_COMPILER_ID:MSVC>:/W4>
      $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )
  endif()
endfunction()
