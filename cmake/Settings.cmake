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

target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)
target_compile_definitions(${target} PRIVATE "PLATFORM_HEADER_NAME=${OS_NAME}")
target_compile_features(${target} PRIVATE cxx_std_23)

SettingFilesTarget(${target})

if (MSVC)
    target_compile_options(${target} PRIVATE /experimental:module)
endif()

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
	SET(MODULES "")
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
	LIST(APPEND Res ${Result})
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
	LIST(APPEND Res ${Result})
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
	LIST(APPEND Res ${Result})
	file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx")
	LIST(APPEND Res ${Result})
		file(GLOB_RECURSE Result RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx")
	LIST(APPEND Res ${Result})

	foreach(file ${Res})
        if("${file}" MATCHES "/Windows/" AND NOT IS_WINDOWS)
            list(REMOVE_ITEM Res ${file})
        elseif("${file}" MATCHES "/Linux/" AND NOT IS_LINUX)
            list(REMOVE_ITEM Res ${file})
		elseif("${file}" MATCHES "/Mac/" AND NOT IS_MAC)
			list(REMOVE_ITEM Res ${file})
        endif()
    endforeach()

	target_sources(${targ} PRIVATE ${Res})
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


function(ADD_PRIVATE_INCLUDE_ONLY target)
	list(SUBLIST ARGV 1 -1 rest)
	foreach(dep IN LISTS rest)
		target_include_directories(${target} PRIVATE $<TARGET_PROPERTY:${dep},INTERFACE_INCLUDE_DIRECTORIES>)

		target_compile_definitions(${target} PRIVATE $<TARGET_PROPERTY:${dep},INTERFACE_COMPILE_DEFINITIONS>)
	endforeach()
endfunction()

function(ADD_PUBLIC_INCLUDE_ONLY target)
	list(SUBLIST ARGV 1 -1 rest)
	foreach(dep IN LISTS rest)
		target_include_directories(${target} PUBLIC $<TARGET_PROPERTY:${dep},INTERFACE_INCLUDE_DIRECTORIES>)

		target_compile_definitions(${target} PUBLIC $<TARGET_PROPERTY:${dep},INTERFACE_COMPILE_DEFINITIONS>)
	endforeach()
endfunction()

function(ADD_DYNAMICLLY_LOADED target)
	list(SUBLIST ARGV 1 -1 rest)
	foreach(dep IN LISTS rest)
		add_dependencies(${target} ${dep})
	endforeach()
endfunction()

function(ADD_PRIVATE_DEFINES target)
	list(SUBLIST ARGV 1 -1 rest)
	foreach(define IN LISTS rest)
        target_compile_definitions(${target} PRIVATE ${define})
    endforeach()
endfunction()

function(ADD_PUBLIC_DEFINES target)
	list(SUBLIST ARGV 1 -1 rest)
	foreach(define IN LISTS rest)
        target_compile_definitions(${target} PUBLIC ${define})
    endforeach()
endfunction()

function(ADD_PUBLIC_LIBRARY target)
	list(SUBLIST ARGV 1 -1 rest)
	foreach(targ IN LISTS rest)
        target_link_libraries(${target} PUBLIC ${targ})
    endforeach()
endfunction()

function(ADD_PRIVATE_LIBRARY target)
	list(SUBLIST ARGV 1 -1 rest)
	foreach(targ IN LISTS rest)
        target_link_libraries(${target} PRIVATE ${targ})
    endforeach()
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


set(PLATFORM_FOLDER "Unknown")
set(OS_NAME "Unknown")
set(IS_WINDOWS FALSE)
set(IS_LINUX FALSE)
set(IS_MAC FALSE)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM_FOLDER "Win64")
	set(OS_NAME "Windows")
	set(IS_WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_FOLDER "Linux")
	set(OS_NAME "Linux")
	set(IS_LINUX TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM_FOLDER "MacOS")
	set(OS_NAME "Mac")
	set(IS_MAC TRUE)
else()
    set(PLATFORM_FOLDER "Unknown")
	set(OS_NAME "Unknown")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires/${PLATFORM_FOLDER}") # 
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires/${PLATFORM_FOLDER}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires/${PLATFORM_FOLDER}")

set(DARK_OUTPUT_FOLDER "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires/${PLATFORM_FOLDER}")

set(CMAKE_INSTALL_PREFIX "${DARK_ENGINE_DIR}/Intermediate/Export" CACHE PATH "Install path" FORCE)

#${projectDir}\Dark Engine\Intermediate\Build\${configurationType}
#set(CMAKE_BINARY_DIR "${DARK_ENGINE_DIR}/Intermediate/Build/${CMAKE_CO}")

# Hot reload
if(MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142) 
	set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Ob0 /Od /RTC1")
endif()
#

add_compile_definitions(IS_ENGINE=1)

set(PROJECT_PREFIX "NewLife")
if(NOT DEFINED PROJECT_TYPE_LINK)
	set(PROJECT_TYPE_LINK "DYNAMIC")
endif()