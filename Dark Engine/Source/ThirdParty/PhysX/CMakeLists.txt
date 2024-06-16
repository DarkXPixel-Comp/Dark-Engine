add_library(PhysX INTERFACE)

target_compile_features(PhysX INTERFACE cxx_std_20)

set(PHYSX_FOLDER_STATIC_LIBS "")



if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	set(PHYSX_FOLDER_STATIC_LIBS "debug")
elseif(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
	set(PHYSX_FOLDER_STATIC_LIBS "profile")
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
	set(PHYSX_FOLDER_STATIC_LIBS "release")
endif()



target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysX_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXCharacterKinematic_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXCommon_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXCooking_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXExtensions_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXFoundation_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXPvdSDK_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXVehicle_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PhysXVehicle2_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/PVDRuntime_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/SnippetRender_static_64.lib")
target_link_libraries(PhysX INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/${PHYSX_FOLDER_STATIC_LIBS}/SnippetUtils_static_64.lib")


target_include_directories(PhysX INTERFACE "include")
