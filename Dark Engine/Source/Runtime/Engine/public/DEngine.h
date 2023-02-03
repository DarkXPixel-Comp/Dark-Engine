#pragma once
#include <Render/Renderer.h>
#include <chrono>
#include <Windows.h>
#include <ApplicationCore/public/Windows/WindowsWindow.h>



#ifdef _WIN64

#elif
#error This OS not support
#endif 






class DEngine
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
	void Tick();
	bool isAppWork();
	void Quit();

	static DEngine* GetEngine();



	


public:
	FWindowsWindowManager* GetWindowManager() { return &m_windowManager; }
	Renderer* GetRenderer() { return m_renderer; }

private:
	FWindowsWindowManager m_windowManager;
	bool m_Quit = false;




	Renderer* m_renderer;




};

extern DEngine GEngine;