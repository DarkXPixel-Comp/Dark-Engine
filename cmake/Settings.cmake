cmake_minimum_required(VERSION 3.8)

#Functions and macros
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

	get_target_property(LIB_TYPE ${target} TYPE)
	set(module_name ${target})
	string(TOUPPER ${module_name} module_name)
if(LIB_TYPE STREQUAL "STATIC_LIBRARY")
	target_compile_definitions(${target} PUBLIC "${module_name}_API= ")
else()
	target_compile_definitions(${target} PRIVATE "${module_name}_API=DLLEXPORT"
	INTERFACE "${module_name}_API=DLLIMPORT")
endif()

target_include_directories(${target} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public> 
        $<INSTALL_INTERFACE:include/${target}>
)
target_include_directories(${target} PRIVATE "Private")

install(TARGETS ${target} EXPORT DarkEngineConfig)
if(EXISTS Public)
install(DIRECTORY Public/ DESTINATION include/${target})
endif()
endfunction()

#cmake_language(GET_COMMAND add_library __add_library)


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


#Includes
include("${CMAKE_CURRENT_LIST_DIR}/CPM.cmake")
set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_LIST_DIR}/../Dark Engine/Intermediate/CPM/")

#Vars
set(DARK_ENGINE_DIR "${CMAKE_CURRENT_LIST_DIR}/../Dark Engine")
set(SUB_PROJECT)

if(DARK_ENGINE_BUILD_TYPE STREQUAL "DYNAMIC")
set(BUILD_SHARED_LIBS ON)
else()
set(BUILD_SHARED_LIBS OFF)
endif()

#set(CMAKE_BINARY_DIR "${DARK_ENGINE_DIR}/TTTT/")

set(CMAKE_INSTALL_PREFIX "${DARK_ENGINE_DIR}/Intermediate/Export" CACHE PATH "Install path" FORCE)

#${projectDir}\Dark Engine\Intermediate\Build\${configurationType}
#set(CMAKE_BINARY_DIR "${DARK_ENGINE_DIR}/Intermediate/Build/${CMAKE_CO}")

# Hot reload
if(MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142) 
	set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Ob0 /Od /RTC1")
endif()
#


set(PROJECT_PREFIX "NewLife")
if(NOT DEFINED PROJECT_TYPE_LINK)
	set(PROJECT_TYPE_LINK "DYNAMIC")
endif()