#include "../public/DEngine.h"
#include <Source/Runtime/Render/D3D12/D3D12Render.h>
#include <iostream>

DEngine GEngine;


#define HINSTANCE() GetModuleHandle(NULL)




void DEngine::Initialize(DEngineInitInfo info)
{
	keyboard = std::make_unique<DirectX::Keyboard>();
	mouse = std::make_unique<DirectX::Mouse>();
	

	Logger::Initialize(LOGGER_ERROR | LOGGER_CONSOLE | LOGGER_INFO);

#ifdef _DEBUG
	CommandConsole::Initialize("Debug Engine");
#endif // _DEBUG
	//InputCore::Initialize();

	


	std::string str = info.nameGame;
	std::wstring nameWindow(str.begin(), str.end());



	WindowManager.CreateWindow(1920, 1080, "Test");


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


void onsize(long w, long h)
{


}


void DEngine::UpdateLoop()
{
	if (!isAppQuit())
	{
		auto time = Clock::now();

		//RenderScene->Update();

	//	if (!Window.isAppQuit())
		{
			WindowManager.Update();
			World->Update();
			Renderer->Render();
			//Window.Update();
		}

		auto time2 = Clock::now();

		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time);

		//deltaTime = static_cast<float>(elapsed.count());

		deltaTime = static_cast<float>(elapsed.count()) / static_cast<float>(1000);


		fps = 1 / (deltaTime / 1000);

		if (WindowManager.GetPrimalWindow())
		{
			string wndTitle = istr(fps) + "  " + istr(WindowManager.GetPrimalWindow()->GetWitdh()) + "  "
				+ istr(WindowManager.GetPrimalWindow()->GetHeight());
			WindowManager.GetPrimalWindow()->SetWindowTitle(wndTitle);
		}





		StartPoint = Clock::now();
	}
	
}


void DEngine::Shutdown()
{
	Logger::log("System succesfully shutdown", LOGGER_INFO);

	Logger::log("", LOGGER_INFO);

}


float DEngine::GetDeltaTime()
{
	return deltaTime;

	auto TimePoint = Clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(TimePoint - StartPoint);

	
	return static_cast<float>(elapsed.count() / 1000.f);


	//return DeltaTime;

}




void DEngine::Quit()
{
	WindowManager.Quit();

	isAppWork = false;

	//PostQuitMessage(0);
}

bool DEngine::isAppQuit()
{
	//x = Window.isAppQuit();
	//y = CommandConsole::isWork();

	//z = isAppWork;


	return (!CommandConsole::isWork()) || !isAppWork;
}