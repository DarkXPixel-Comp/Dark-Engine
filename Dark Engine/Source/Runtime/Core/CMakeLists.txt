add_library(Core)

find_library(CoreLibs DTK12)

set(CMAKE_CXX_STANDARD 20)
target_compile_features(Core PUBLIC cxx_std_20)

target_link_libraries(Core PRIVATE DirectXMath)
target_link_libraries(Core PUBLIC CoreGObject Optick)
#target_link_libraries(Core PRIVATE ${CoreLibs})

set(VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(VERSION_PREFIX alpha)
set(BUILD_TYPE_PREFIX ${CMAKE_BUILD_TYPE})

if(WIN32)
	target_sources(Core PRIVATE "Public/Windows/WindowsPlatformMisc.h" "Private/Windows/WindowsPlatformMisc.cpp" "Public/Windows/WindowsPlatformMemory.h" "Private/Windows/WindowsPlatformMemory.cpp" "Public/Windows/WindowsPlatformString.h" "Private/Windows/WindowsCriticalSection.cpp") # Windows
	#set(CORE_SOURCES ${CORE_SOURCES} "Public/Windows/WindowsPlatformMisc.h" "Private/Windows/WindowsPlatformMisc.cpp" "Public/Windows/WindowsPlatformMemory.h" "Private/Windows/WindowsPlatformMemory.cpp")
endif()

configure_file("Private/Misc/EngineVersion.cpp.var" "${CMAKE_CURRENT_SOURCE_DIR}/Private/Misc/EngineVersion.cpp" @ONLY)

#set(CORE_SOURCES ${CORE_SOURCES} "Private/CoreGlobals.cpp"
#"Private/Misc/AssertionMacros.cpp" "Private/Math/DarkMath.cpp" "Application/Application.cpp" "Private/Misc/CoreDelegates.cpp"
#"Logging/Logger.cpp" "CommandLine/CommandLine.cpp" "Timer/GameTimer.cpp" "Memory/TMemory.cpp" "Public/Misc/Paths.h" "Private/Misc/Paths.cpp" "Public/GenericPlatform/GenericPlatformMisc.h" "Public/HAL/PlatformMisc.h" "Private/GenericPlatform/GenericPlatformMisc.cpp" "Private/Containers/DarkString.cpp" "Private/GenericPlatform/GenericPlatformMemory.cpp"
#"Public/HAL/DarkMemory.h" "Public/HAL/FMemory.inl")

#GroupSourcesByFolder(Core "${CORE_SOURCES}")

target_sources(Core PRIVATE "Private/CoreGlobals.cpp"
"Private/Misc/AssertionMacros.cpp" "Private/Math/DarkMath.cpp" "Application/Application.cpp" "Private/Misc/CoreDelegates.cpp"
"Logging/Logger.cpp" "CommandLine/CommandLine.cpp" "Timer/GameTimer.cpp" "Memory/TMemory.cpp" "Public/Misc/Paths.h" "Private/Misc/Paths.cpp" "Public/GenericPlatform/GenericPlatformMisc.h" "Public/HAL/PlatformMisc.h" "Private/GenericPlatform/GenericPlatformMisc.cpp" "Private/Containers/DarkString.cpp" "Private/GenericPlatform/GenericPlatformMemory.cpp"
"Public/HAL/DarkMemory.h" "Public/HAL/FMemory.inl" "Public/Containers/List.h" "Public/Math/Matrix.h" "Public/Math/IntRect.h" "Public/Math/IntPoint.h" "Public/Templates/IsInteger.h" "Public/Math/PersperctiveMatrix.h" "Public/Math/Plane.h" "Public/Containers/UnordoredMap.h" "Public/Containers/StaticArray.h" "Public/Containers/Array.h" "Logging/Logger.h" "Public/Logging/LogMacros.h" "Public/Logging/LogCategory.h" "Public/Logging/StructedLog.h" "Private/Logging/StructedLog.cpp" "Private/Logging/LogMarcros.cpp" "Public/Misc/VarArgs.h" "Public/Templates/RefCounting.h" "Public/Containers/BitArray.h" "Public/Templates/AlignTemplate.h" "Public/Console/ConsoleStd.h" "Private/Console/BaseConsole.cpp" "Private/Console/ConsoleStd.cpp" "Private/Console/GlobalInputOutConsole.cpp" "Public/Windows/WindowsCriticalSection.h" "Public/HAL/CriticalSection.h" "Public/Misc/ScopeLock.h" "Public/Math/Vector4.h" "Public/Misc/Config.h" "Private/Misc/Config.cpp" "Public/Misc/EngineVersion.h" "Private/Misc/EngineVersion.cpp" "Public/Math/InverseRotationMatrix.h" "Public/Math/MathSSE.h" "Private/Math/MathSSE.cpp" "../CoreGObject/public/Templates/Casts.h" "Public/Math/Transform.h" "Public/Math/Color.h" "Public/Windows/WindowsProcess.h" "Public/Misc/ProcessManager.h" "Private/Misc/ProcessManager.cpp")

SettingProject(Core)

target_include_directories(Core PUBLIC "Public" "Timer" "Settings" "Memory" "Logging" "Delegate" "CommandLine" "Application" "../Core")

#install(TARGETS Core)