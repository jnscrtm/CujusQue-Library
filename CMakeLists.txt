﻿cmake_minimum_required (VERSION 3.12)
set(CMAKE_LANG)
set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Cujusque" CXX)

include_directories("${Cujusque_SOURCE_DIR}/include")

add_library(Cujusque "corelib.cpp" "source/Any.cpp" "source/TypeTag.cpp")
set_target_properties(Cujusque PROPERTIES PUBLIC_HEADER "${Cujusque_SOURCE_DIR}/include/*.hpp")

add_subdirectory("tester")

install(TARGETS Cujusque PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")