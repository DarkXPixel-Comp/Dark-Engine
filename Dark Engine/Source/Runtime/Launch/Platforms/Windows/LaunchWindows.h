#pragma once
#include <Core/Core.h>
#include <Core/Settings/PerEngineSettings.h>
#include <Launch/Resource/resource.h>
#include <Core/Application/Application.h>
#include <Engine/public/DEngine.h>
#include <Core/Logging/Logger.hpp>




void DENGINE_API WindowsEnviromentSetup();




int32_t DENGINE_API WWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE prevInstance,
	_In_ char* lpCmdLine,
	_In_ int32_t nCmdShow
);