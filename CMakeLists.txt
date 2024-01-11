cmake_minimum_required(VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Dark Engine")

add_compile_definitions(UBT_COMPILED_PLATFORM=Windows ROOT_DIR="${PROJECT_SOURCE_DIR}")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Dark Engine/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Dark Engine/bin/lib")

add_subdirectory("Dark Engine")