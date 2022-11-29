#pragma once
#include <Source/Runtime/Core/Core.h>
#include <Source/Runtime/Render/Renderer.h>
#include <World/World.h>
//#include "World/World.h"
#include <Keyboard.h>
#include <Mouse.h>
#include <chrono>






struct DEngineInitInfo
{
	const char* nameGame;
	uint32_t platformCmdShow;

	struct WindowInitInfo
	{
		uint32_t width;
		uint32_t height;
		uint32_t leftX;
		uint32_t topY;



	} WindowInitInfo;



};

#ifdef _WIN64
using platformWindow = WindowsWindow;

#elif
#error This OS not support
#endif 







struct FScreenMessage
{
	std::string ScreenMessage;

	float TimeDisplay;

	float CurrentTimeDisplayed;


};




#define _inst this



using DirectX::Keyboard;
using DirectX::Mouse;

class DEngine
{
	typedef std::chrono::high_resolution_clock Clock;

public:
	DEngine() = default;

	void Initialize(DEngineInitInfo info);
	void UpdateLoop();
	void Shutdown();
	void Quit();


public:
	bool isAppQuit();
	platformWindow* GetWindow();

	Renderer* GetRenderer() { return Renderer; }

	AWorld* GetWorld() { return World; }
	//FRenderScene* GetScene() { return RenderScene; }


	Keyboard* GetKeyboard() { return keyboard.get(); }
	Mouse* GetMouse() { return mouse.get(); }

	float GetDeltaTime() { return deltaTime; }
	int GetFPS() { return fps; }
	

//	static World& GetWorld();

private:
	DEngineInitInfo InitInfo;
	bool isAppWork = false;


	platformWindow Window;
	Renderer* Renderer;
	AWorld* World;
	//FRenderScene* RenderScene;
	std::unique_ptr<DirectX::Keyboard> keyboard;
	std::unique_ptr<DirectX::Mouse> mouse;
	
	float deltaTime = 0.f;
	int fps = 0;




};

extern DEngine GEngine;

