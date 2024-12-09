cmake_minimum_required(VERSION 3.8)


#Functions
function(SettingProject target)
  set(SOURCE_GROUP_DELIMITER "/") 
  set(last_dir "")
  set(files "")


  get_target_property(sources ${target} SOURCES)
  foreach(file ${sources})
	set(t1 "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
	file(RELATIVE_PATH relative_file "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${file}") 
	get_filename_component(dir "${relative_file}" PATH)               
	if(NOT "${dir}" STREQUAL "${last_dir}")
	  if(files)
		source_group("${last_dir}" FILES ${files})
	  endif()
	  set(files "")
	endif()
	set(files ${files} ${file})
	set(last_dir "${dir}")
  endforeach()

  if(files)
	source_group("${last_dir}" FILES ${files})
  endif()

	file(RELATIVE_PATH cmake_file "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt")
	get_filename_component(dir "${cmake_file}" PATH)
	set_target_properties(${target} PROPERTIES FOLDER ${dir})
	target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}/Dark Engine/Source")
endfunction()

macro(SettingFiles Out)
	SET(Res "")
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
	LIST(APPEND Res ${Result})
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
	LIST(APPEND Res ${Result})
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
	LIST(APPEND Res ${Result})
	set(${Out} ${Res})
endmacro()

macro(SettingFilesTarget targ)
	SET(Res "")
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
	LIST(APPEND Res ${Result})
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
	LIST(APPEND Res ${Result})
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
	LIST(APPEND Res ${Result})
	target_sources(${targ} PRIVATE ${Res}})
endmacro()

function(GroupSourcesByFolder target target_files)
  set(SOURCE_GROUP_DELIMITER "/") 
  set(last_dir "")
  set(files "")

  foreach(src ${target_files})
  target_sources(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
  endforeach()

  get_target_property(sources ${target} SOURCES)
  foreach(file ${sources})
	file(RELATIVE_PATH relative_file "${CMAKE_CURRENT_SOURCE_DIR}" ${file}) 
	get_filename_component(dir "${relative_file}" PATH)               
	if(NOT "${dir}" STREQUAL "${last_dir}")
	  if(files)
		source_group("${last_dir}" FILES ${files})
	  endif()
	  set(files "")
	endif()
	set(files ${files} ${file})
	set(last_dir "${dir}")
  endforeach()

  if(files)
	source_group("${last_dir}" FILES ${files})
  endif()

	file(RELATIVE_PATH cmake_file "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt")
	get_filename_component(dir "${cmake_file}" PATH)
	set_target_properties(${target} PROPERTIES FOLDER ${dir})
endfunction()




MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

#Includes
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")

#Vars
set(PROJECT_PREFIX "NewLife")

# Hot reload
if(MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
	set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Ob0 /Od /RTC1")
endif()
#


add_compile_definitions(UBT_COMPILED_PLATFORM=Windows ROOT_DIR="${PROJECT_SOURCE_DIR}")


#Output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<0:>${PROJECT_SOURCE_DIR}/Dark Engine/Binaires") # 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/Libs")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/Libs")

#Dark Engine
project("Dark Engine" VERSION 0.1.3.0)
add_subdirectory("Dark Engine")


