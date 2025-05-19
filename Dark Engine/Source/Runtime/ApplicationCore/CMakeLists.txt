add_library(ApplicationCore)

ADD_PUBLIC_LIBRARY(ApplicationCore Core)

if(WIN32)
	ADD_PRIVATE_LIBRARY(ApplicationCore dwmapi)
endif()

SettingProject(ApplicationCore)
