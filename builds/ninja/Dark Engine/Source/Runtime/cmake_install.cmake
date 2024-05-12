# Install script for directory: C:/Users/nahmu/source/repos/Dark Engine/Dark Engine/Source/Runtime

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Dark Engine")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/ApplicationCore/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Core/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/CoreDObject/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/D3D12RHI/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Editor/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Engine/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Input/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Launch/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/RHI/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Render/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Renderer/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/UICore/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/VulkanRHI/cmake_install.cmake")
  include("C:/Users/nahmu/source/repos/Dark Engine/builds/ninja/Dark Engine/Source/Runtime/Windows/cmake_install.cmake")

endif()

