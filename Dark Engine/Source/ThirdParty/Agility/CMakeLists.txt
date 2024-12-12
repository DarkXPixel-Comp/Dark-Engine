add_library(Agility STATIC)

CPMADDPACKAGE(NAME AgilitySDK
	URL "https://www.nuget.org/api/v2/package/Microsoft.Direct3D.D3D12/1.614.1")

if(AgilitySDK_ADDED)
	target_sources(Agility PRIVATE "${AgilitySDK_SOURCE_DIR}/build/native/src/d3dx12/d3dx12_property_format_table.cpp")		
	target_include_directories(Agility PUBLIC "${AgilitySDK_SOURCE_DIR}/build/native/include")
	target_include_directories(Agility PUBLIC "${AgilitySDK_SOURCE_DIR}/build/native/include/d3dx12")

	file(COPY "${AgilitySDK_SOURCE_DIR}/build/native/bin/x64/D3D12Core.dll" 
		DESTINATION "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires/D3D12/")
	file(COPY "${AgilitySDK_SOURCE_DIR}/build/native/bin/x64/d3d12SDKLayers.dll" 
		DESTINATION "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires/D3D12/")
endif()