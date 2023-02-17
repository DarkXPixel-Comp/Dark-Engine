#include "../public/DEngine.h"
#include <Render/D3D12/D3D12Scene.h>
#include <Core/Logging/Logger.hpp>
#include <Core/Timer/GameTimer.h>
#include <Core/CoreDefines.h>

DEngine GEngine;


#define HINSTANCE() GetModuleHandle(NULL)


DEngine::~DEngine()
{
	Shutdown();
}



int32_t DEngine::Initialize()
{
	Logger::Initialize(LOGGER_INFO | LOGGER_ERROR | LOGGER_WARNING | LOGGER_CONSOLE);
	Log("DEngine Started Initialize");

	CommandConsole::Initialize("CommandConsole");

	m_input = std::make_unique<FInputCore>();
	m_windowManager.CreateWindow(1920, 1080, "DEngine");
	m_input->SetWindow(m_windowManager.GetWindow(0)->GetHandle());

	m_renderer = std::make_unique<RENDER_API>();
	m_renderer->Init();
	m_scene = std::make_unique<D3D12Scene>();
	m_world = std::make_unique<AWorld>();
	m_world->Init();

	FGameTimer::Reset();
	FGameTimer::Tick();



	return 0;

}


int32_t DEngine::PostInit()
{
	m_windowManager.SetDelay(16);	


	auto mesh = D3DUtil::LoadMesh("Models/cube.obj");


	for (size_t i = 0; i < 1; i++)
	{
		auto actor = m_world->CreateActor();
		actor->SetPosition({ (float)i, 0, 5 });
		actor->SetMesh(mesh);
	}

	m_world->GetCamera()->SetRotation({0, 0, 90.f});
	m_world->GetCamera()->SetupPlayerController(m_input.get());
	m_input->EscDelegate.Bind(this, &DEngine::Quit);


	return 0;
}



bool DEngine::isAppWork()
{
	/*bool x1 = CommandConsole::isWork();
	bool x2 = m_Quit;
	bool x3 = m_windowManager.WindowsIsClose();*/

	return !(m_Quit || !(CommandConsole::isWork() || !m_windowManager.WindowsIsClose()));

}


void DEngine::SetDelayUpdate(int DelayMs)
{
	m_windowManager.SetDelay(DelayMs < 1 ? 1 : DelayMs);
}

void DEngine::SetMaxFPS(int fps)
{
	m_windowManager.SetDelay(fps > 1000 ? 1 : 1000 / fps);
}




void DEngine::UpdateLoop()
{
	float deltaTime = FGameTimer::DeltaTime();

	m_windowManager.Update();
	m_input->Update();
	if (m_windowManager.WindowsIsClose()) return;

	m_world->Update(deltaTime);
	m_world->FillScene(m_scene.get());

	m_renderer->Render(m_scene.get());

	Tick();

	FGameTimer::Tick();
	 
}


DEngine* DEngine::GetEngine()
{
	return &GEngine;
}



void DEngine::Tick()
{
	{
		m_windowManager.GetWindow(0)->SetWindowTitle(icstr(1 / FGameTimer::DeltaTime()));
	}
}


void DEngine::Quit()
{
	m_windowManager.Quit();

	m_Quit = true;




}


void DEngine::Shutdown()
{
	if (!m_Quit)
	{
		Quit();
	}

}


