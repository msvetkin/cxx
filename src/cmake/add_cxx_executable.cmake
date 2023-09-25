# SPDX-FileCopyrightText: Copyright 2023 Mikhail Svetkin
# SPDX-License-Identifier: MIT

include_guard(GLOBAL)

include(set_cxx_target_properties)

# sets all nessary default things
function(add_cxx_executable executable_name)
  set(executable_target cxx-${executable_name})

  add_executable(${executable_target} ${ARGN})
  set_cxx_target_properties(${executable_target} PRIVATE)

  if (TARGET cxx)
    install(TARGETS ${executable_target} EXPORT cxx-targets)
  endif()

  set(cxx_executable_target
      ${executable_target}
      PARENT_SCOPE
  )
endfunction()
