#pragma once
#include <Core.h>
#include "Renderer.h"
#include <Windows/WindowsWindow.h>
#include <Delegate/Delegate.h>
#include <Memory/TUniquePtr.h>
#include <World.h>
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






class DENGINE_API DEngine
{
	typedef std::chrono::high_resolution_clock Clock;

public:
	DEngine() = default;
	~DEngine();

	int32_t Initialize();
	int32_t PostInit();
	void UpdateLoop();
	void Shutdown();
	void SetDelayUpdate(int DelayMs);
	void SetMaxFPS(int fps);
	void Tick(float deltaTime);
	bool isAppWork();
	void Quit();
	static DEngine* GetEngine();
//	static void SetEditor(DEditor* edt);

	FOnTick onTick;
	FOnRenderInterface OnRenderInterface;




public:
	FWindowsWindowManager* GetWindowManager() { return &m_windowManager; }
	Renderer* GetRenderer() { return m_renderer.get(); }
	TMemory* GetMemory() { return m_memory.get(); }
	AWorld* GetWorld() { return m_world.get(); }
	FInputCore* GetInput() { return m_input.get(); }
	D3D12Scene* GetScene() { return m_scene.get(); }

private:
	/*DEditor* m_editor;
	DEditor m_defaultEditor;*/
	FWindowsWindowManager m_windowManager;
	bool m_Quit = false;
	bool renderRun = false;




	//Renderer* m_renderer;
	TUniquePtr<Renderer> m_renderer;
	TUniquePtr<D3D12Scene> m_scene;
	TUniquePtr<AWorld> m_world;
	TUniquePtr<FInputCore> m_input;
	TUniquePtr<TMemory> m_memory;


private:
	void SetFullscreen();
	void Interface();




};

extern DEngine GEngine;