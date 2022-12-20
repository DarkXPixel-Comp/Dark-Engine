#include "../public/DEngine.h"
#include <Source/Runtime/Render/D3D12/D3D12Render.h>

DEngine GEngine;


#define HINSTANCE() GetModuleHandle(NULL)


void DEngine::Initialize(DEngineInitInfo info)
{
	Logger::Initialize(LOGGER_ERROR | LOGGER_CONSOLE | LOGGER_INFO);

#ifdef _DEBUG
	CommandConsole::Initialize("Debug Engine");
#endif // _DEBUG
	//InputCore::Initialize();


	std::string str = info.nameGame;
	std::wstring nameWindow(str.begin(), str.end());

	Window.Initialize(HINSTANCE(),
		info.platformCmdShow,
		nameWindow.c_str(),
		info.WindowInitInfo.leftX, 
		info.WindowInitInfo.topY,
		info.WindowInitInfo.width,
		info.WindowInitInfo.height);

//	Window.SetResolution(1600, 900);

	//Window.Minimize();

	keyboard = std::make_unique<DirectX::Keyboard>();
	mouse = std::make_unique<DirectX::Mouse>();


	
	//mouse->SetWindow(Window.GetHandle());
	//mouse->SetMode(Mouse::MODE_RELATIVE);
	//mouse->SetVisible(true);
	


	Renderer = new RENDER_API;

	Renderer->Init();

	World = new AWorld;
	//RenderScene = new FRenderScene;

	World->Initialize();
	//RenderScene->Initalize();

	//RenderScene->SetCamera(World->GetCamera());

	InitInfo = info;

	isAppWork = true;



	StartPoint = Clock::now();
}



platformWindow* DEngine::GetWindow()
{
	return &Window;
}

int _fps()
{
	static int fps = 0;
	static auto lastTime = std::chrono::high_resolution_clock::now(); // ms
	static int frameCount = 0;

	++frameCount;

	auto curTime = std::chrono::high_resolution_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTime);
	if (elapsed.count() > 1000) // take a fixed time interval of 1 second
	{
		fps = frameCount;
		frameCount = 0;
		lastTime = curTime;
	}
	return fps;

}




void DEngine::UpdateLoop()
{
	auto time = Clock::now();

	//RenderScene->Update();

	if (!Window.isAppQuit())
	{
		World->Update();
		Renderer->Render();
		Window.Update();
	}
	

	auto time2 = Clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time);

	//deltaTime = static_cast<float>(elapsed.count());

	deltaTime = static_cast<float>(elapsed.count()) / static_cast<float>(1000);

	fps = _fps();

	StartPoint = Clock::now();
}


void DEngine::Shutdown()
{
	Window.CloseWindow();

	Logger::log("System succesfully shutdown", LOGGER_INFO);

	Logger::log("", LOGGER_INFO);

}


float DEngine::GetDeltaTime()
{
	auto TimePoint = Clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(TimePoint - StartPoint);

	
	return static_cast<float>(elapsed.count() / 1000.f);


	//return DeltaTime;

}




void DEngine::Quit()
{
	isAppWork = false;
}

bool DEngine::isAppQuit()
{
	//x = Window.isAppQuit();
	//y = CommandConsole::isWork();

	//z = isAppWork;


	return (Window.isAppQuit() && !CommandConsole::isWork()) || !isAppWork;
}