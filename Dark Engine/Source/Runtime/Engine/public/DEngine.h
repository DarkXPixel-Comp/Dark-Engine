#pragma once
#include <Core.h>
#include "D3D12/Renderer.h"
#include <Windows/WindowsWindow.h>
#include <Delegate/Delegate.h>
#include <Memory/TUniquePtr.h>
#include <InputCore.h>
#include <Memory/TMemory.h>
#include "DEditor.h"
#include <chrono>
#include <Windows.h>
#include <memory>

#ifdef _WIN64

#elif
#error This OS not support
#endif 


DECLARE_MULTICAST_DELEGATE_OneParam(FOnTick, float);
DECLARE_MULTICAST_DELEGATE(FOnRenderInterface);



