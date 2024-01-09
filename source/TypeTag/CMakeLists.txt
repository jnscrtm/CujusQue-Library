cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Cujusque.TypeTag")

include_directories("${Cujusque_SOURCE_DIR}/include")
add_library(TypeTag STATIC "TypeTag.cpp")
set_target_properties(TypeTag PROPERTIES PUBLIC_HEADER "${Cujusque_SOURCE_DIR}/include/TypeTag.hpp")

install(TARGETS TypeTag PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")