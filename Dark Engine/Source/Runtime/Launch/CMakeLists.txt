add_executable(Launch)

SETTINGPROJECT(Launch)
ADD_PRIVATE_LIBRARY(Launch Core ApplicationCore DarkUI ImGui JSON Agility PhysFS)
ADD_DYNAMICLLY_LOADED(Launch TestModule)

set_target_properties(Launch PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")

if(WIN32)
	set_target_properties(Launch PROPERTIES WIN32_EXECUTABLE 1)
	target_sources(Launch PRIVATE "Resources/Windows/app.rc")
endif()
