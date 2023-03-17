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


	auto PersonMesh = D3DUtil::LoadMesh("Models/objectSphere.obj");
	auto CubeMesh = D3DUtil::LoadMesh("Models/dev.fbx");
	auto FloorMesh = D3DUtil::LoadMesh("Models/floor.obj");
	auto SphereMesh = D3DUtil::LoadMesh("Models/m9.fbx");

	{
		auto FloorActor = m_world->CreateActor();
		FloorActor->SetPosition({ 0, 0, 0 });
		FloorActor->SetScale({ 10, 0, 10 });
		FloorActor->SetRotation({0, 0, 0});
		FloorActor->SetMesh(FloorMesh);

		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		material->m_frenselR0 = { 0.1f, 0.1f, 0.1f };
		material->m_roughness = 1.0f;

		FloorActor->GetModel()->SetMaterial(material);
	}


	{
		auto CubeActor = m_world->CreateActor();
		CubeActor->SetPosition({ 0, 0, 5 });
		CubeActor->SetScale({0.015, 0.015, 0.015});
		CubeActor->SetMesh(CubeMesh);

		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(0.5f, 0.7f, 0.6f, 1.0f);
		material->m_frenselR0 = { 0.1f, 0.1f, 0.1f };
		material->m_roughness = 0.5f;

		CubeActor->GetModel()->SetMaterial(material);
	}

	{
		auto SphereActor = m_world->CreateActor();
		SphereActor->SetPosition({ 3, 0, 5 });
		SphereActor->SetMesh(PersonMesh);

		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(0.5f, 0.2f, 0.6f, 1.0f);
		material->m_frenselR0 = { 0.1f, 0.1f, 0.1f };
		material->m_roughness = 0.5f;

		SphereActor->GetModel()->SetMaterial(material);

	}


	{
		auto SphereActor = m_world->CreateActor();
		SphereActor->SetPosition({ 2, 2, 3 });
		SphereActor->SetMesh(SphereMesh);
		SphereActor->SetScale({ 10, 10, 10 });
		SphereActor->SetRotation({ 0, 0, 0 });

		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(1.f, 1.0f, 1.0f, 1.0f);
		material->m_frenselR0 = { 1.0f, 1.0f, 1.0f };
		material->m_roughness = 0.0f;

		SphereActor->GetModel()->SetMaterial(material);

		onTick.Bind(SphereActor, &AActor::OnUpdateRotation);
	}

	m_world->GetCamera()->SetRotation({0, 0, 90.f});
	m_world->GetCamera()->SetupPlayerController(m_input.get());
	m_input->EscDelegate.Bind(this, &DEngine::Quit);

	m_renderer->SetVsync(1);



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

	Tick(deltaTime);

	FGameTimer::Tick();
	 
}


DEngine* DEngine::GetEngine()
{
	return &GEngine;
}



void DEngine::Tick(float deltaTime)
{
	onTick.BroadCast(deltaTime);
	m_windowManager.GetWindow(0)->SetWindowTitle(icstr(1 / deltaTime));

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


