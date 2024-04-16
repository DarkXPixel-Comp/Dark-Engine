add_library(Core)

find_library(CoreLibs DTK12)

set(CMAKE_CXX_STANDARD 20)
target_compile_features(Core PUBLIC cxx_std_23)

target_link_libraries(Core PRIVATE DTK12)
#target_link_libraries(Core PRIVATE ${CoreLibs})


if(MSVC AND WIN32)
	target_sources(Core PRIVATE "Public/Windows/WindowsPlatformMisc.h" "Private/Windows/WindowsPlatformMisc.cpp" "Public/Windows/WindowsPlatformMemory.h" "Private/Windows/WindowsPlatformMemory.cpp" "Public/Windows/WindowsPlatformString.h" "Private/Windows/WindowsCriticalSection.cpp") # Windows
	#set(CORE_SOURCES ${CORE_SOURCES} "Public/Windows/WindowsPlatformMisc.h" "Private/Windows/WindowsPlatformMisc.cpp" "Public/Windows/WindowsPlatformMemory.h" "Private/Windows/WindowsPlatformMemory.cpp")

endif()

#set(CORE_SOURCES ${CORE_SOURCES} "Private/CoreGlobals.cpp"
#"Private/Misc/AssertionMacros.cpp" "Private/Math/DarkMath.cpp" "Application/Application.cpp" "Private/Misc/CoreDelegates.cpp"
#"Logging/Logger.cpp" "CommandLine/CommandLine.cpp" "Timer/GameTimer.cpp" "Memory/TMemory.cpp" "Public/Misc/Paths.h" "Private/Misc/Paths.cpp" "Public/GenericPlatform/GenericPlatformMisc.h" "Public/HAL/PlatformMisc.h" "Private/GenericPlatform/GenericPlatformMisc.cpp" "Private/Containers/DarkString.cpp" "Private/GenericPlatform/GenericPlatformMemory.cpp"
#"Public/HAL/DarkMemory.h" "Public/HAL/FMemory.inl")

#GroupSourcesByFolder(Core "${CORE_SOURCES}")

target_sources(Core PRIVATE "Private/CoreGlobals.cpp"
"Private/Misc/AssertionMacros.cpp" "Private/Math/DarkMath.cpp" "Application/Application.cpp" "Private/Misc/CoreDelegates.cpp"
"Logging/Logger.cpp" "CommandLine/CommandLine.cpp" "Timer/GameTimer.cpp" "Memory/TMemory.cpp" "Public/Misc/Paths.h" "Private/Misc/Paths.cpp" "Public/GenericPlatform/GenericPlatformMisc.h" "Public/HAL/PlatformMisc.h" "Private/GenericPlatform/GenericPlatformMisc.cpp" "Private/Containers/DarkString.cpp" "Private/GenericPlatform/GenericPlatformMemory.cpp"
"Public/HAL/DarkMemory.h" "Public/HAL/FMemory.inl" "Public/Containers/List.h" "Public/Math/Matrix.h" "Public/Math/IntRect.h" "Public/Math/IntPoint.h" "Public/Templates/IsInteger.h" "Public/Math/PersperctiveMatrix.h" "Public/Math/Plane.h" "Public/Containers/UnordoredMap.h" "Public/Containers/StaticArray.h" "Public/Containers/Array.h" "Logging/Logger.h" "Public/Logging/LogMacros.h" "Public/Logging/LogCategory.h" "Public/Logging/StructedLog.h" "Private/Logging/StructedLog.cpp" "Private/Logging/LogMarcros.cpp" "Public/Misc/VarArgs.h" "Public/Templates/RefCounting.h" "Public/Containers/BitArray.h" "Public/Templates/AlignTemplate.h" "Public/Console/ConsoleStd.h" "Private/Console/BaseConsole.cpp" "Private/Console/ConsoleStd.cpp" "Private/Console/GlobalInputOutConsole.cpp" "Public/Windows/WindowsCriticalSection.h" "Public/HAL/CriticalSection.h" "Public/Misc/ScopeLock.h")

SettingProject(Core)

target_include_directories(Core PUBLIC "Public" "Timer" "Settings" "Memory" "Logging" "Delegate" "CommandLine" "Application" "../Core")

#install(TARGETS Core)