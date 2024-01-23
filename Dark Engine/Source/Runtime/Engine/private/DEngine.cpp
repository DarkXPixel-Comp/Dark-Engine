#include "../public/DEngine.h"
#include "D3D12/D3D12Scene.h"
#include <Logging/Logger.hpp>
#include <Timer/GameTimer.h>
#include <CoreDefines.h>
#include <TestActor/TestActor.h>
#include <Application/Application.h>
#include <Components/SceneComponent/USceneComponent.h>
#include "Math/MathFwd.h"
#include <RHI.h>
#include "Misc/Paths.h"
//#include "RHI.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "D3D12/D3D12Utils.h"
#include "D3D12/D3D12Material.h"


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
  	m_memory = std::make_unique<TMemory>();
	m_input = std::make_unique<FInputCore>();
	m_windowManager.CreateWindow(1920, 1080, "DEngine");
	m_input->SetWindow(m_windowManager.GetWindow(0)->GetHandle());

	m_renderer = std::make_unique<RENDER_API>();
	m_renderer->Init();
	m_scene = std::make_unique<D3D12Scene>();
	m_world = std::make_unique<AWorld>();
	m_world->Init();

	//RHIInit();

	FVector vec;

	//if (!m_editor)
	//	m_editor = &m_defaultEditor;

	FGameTimer::Reset();
	FGameTimer::Tick();

	return 0;

}

void ThreadRender()
{

}

int32_t DEngine::PostInit()
{
	m_windowManager.SetDelay(32);	


	

	auto PersonMesh = D3DUtil::LoadMesh(FPaths::CombineDir(FPaths::EngineContentDir(), "Meshes/kanistra.fbx"));
	auto DiffuseKanistra = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/kanistra.dds"));
	auto NormalKanistra = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/kanistra_normal.dds"));
	auto GrassDiff = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/sandDif.dds"));
	auto GrassNorm = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/sandNor.dds"));


	auto CubeMesh = D3DUtil::LoadMesh(FPaths::CombineDir(FPaths::EngineContentDir(), "Meshes/dev.fbx"));
	auto FloorMesh = D3DUtil::LoadMesh(FPaths::CombineDir(FPaths::EngineContentDir(), "Meshes/floor.obj"));
	auto SphereMesh = D3DUtil::LoadMesh(FPaths::CombineDir(FPaths::EngineContentDir(), "Meshes/m9.fbx"));
	auto UVTexture = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/uv.dds"));

	auto CubeTexture = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/Super.dds"));
	auto NormalMap = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/SuperNormal.dds"));
	



	{
		auto FloorActor = m_world->CreateActor();
		FloorActor->SetPosition({ 0, 0, 0 });
		FloorActor->SetScale({ 10, 0, 10 });
		FloorActor->SetRotation({0, 0, 0});
		FloorActor->SetMesh(FloorMesh);
		FloorActor->SetName("Floor");

		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		material->m_frenselR0 = { 0.1f, 0.1f, 0.1f };
		material->m_roughness = 1.0f;
		material->t_Albedo = GrassDiff;
		material->t_Normal = NormalMap;

		

		FloorActor->GetModel()->SetMaterial(material);
	}


	{
		auto CubeActor = m_world->CreateActor<ATestActor>();
		CubeActor->SetPosition({ 0, 0, 5 });
		CubeActor->SetScale({0.015, 0.015, 0.015});
		CubeActor->SetMesh(CubeMesh);
		CubeActor->SetName("Person");

		auto Test = Cast<ATestActor>(CubeActor);

		Test->rotateComponent->SetRotateOnTick({ 10, 0, 0 });


		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(0.5f, 0.7f, 0.6f, 1.0f);
		material->m_frenselR0 = { 0.1f, 0.1f, 0.1f };
		material->m_roughness = 0.5f;

		CubeActor->GetModel()->SetMaterial(material);
	}

	{
		auto SphereActor = m_world->CreateActor();
		SphereActor->SetPosition({ 3, 2, 5 });
		SphereActor->SetScale({ 4, 4, 4 });
		SphereActor->SetRotation({ 190, 0, 0 });
		SphereActor->SetMesh(PersonMesh);
		SphereActor->SetName("Fuel");

		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(0.5f, 0.2f, 0.6f, 1.0f);
		material->m_frenselR0 = { 0.1f, 0.1f, 0.1f };
		material->m_roughness = 0.5f;
		material->t_Albedo = DiffuseKanistra;
		material->t_Normal = NormalKanistra;
		SphereActor->GetModel()->SetMaterial(material);

	}


	{
		auto SphereActor = m_world->CreateActor();
		SphereActor->SetPosition({ 2, 2, 3 });
		SphereActor->SetMesh(SphereMesh);
		SphereActor->SetScale({ 10, 10, 10 });
		SphereActor->SetRotation({ 0, 0, 0 });
		SphereActor->SetName("Knife");

		auto material = new D3D12Material();
		material->m_diffuseAlbedo = XMFLOAT4(1.f, 1.0f, 1.0f, 1.0f);
		material->m_frenselR0 = { 1.0f, 1.0f, 1.0f };
		material->m_roughness = 0.0f;
		material->t_Albedo = CubeTexture;

		SphereActor->GetModel()->SetMaterial(material);
	}

	m_world->GetCamera()->SetRotation({0, 0, 90.f});
	m_world->GetCamera()->SetupPlayerController(m_input.get());
	//m_world->GetCamera()->GetCamera().m_fov = 80;
	m_input->EscDelegate.Bind(this, &DEngine::Quit);
	m_input->F11Delegate.Bind(this, &DEngine::SetFullscreen);
	OnRenderInterface.Bind(this, &DEngine::Interface);


	m_renderer->SetVsync(0);

	/*m_editor->Init();*/

	//SetMaxFPS(500);

	/*thread th(ThreadRender);
	th.detach();*/

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

void DEngine::SetFullscreen()
{
	static bool FullScreen = true;
	m_renderer->SetResolution(1920, 1080, FullScreen);
	FullScreen = !FullScreen;

}




void AddInterface(TArray<FObjectProperties> Properties)
{
	for (auto& i : Properties)
	{
		TArray<FObjectProperties> prop;
		switch (i.TypeProperties)
		{
		case None:
			break;
		case Integer:
			break;
		case Float:
			break;
		case String:
			break;
		case Pointer:
			break;
		case Vector2:
			break;
		case Vector3:
			ImGui::DragFloat3(i.Name.c_str(), (float*)i.Properties);
			break;
		case Vector4:
			break;
		case Array:
			break;
		case Custom:
			ImGui::SeparatorText(i.Name.c_str());
			prop = reinterpret_cast<UObject*>(i.Properties)->GetProperties();
			AddInterface(prop);
			break;
		default:
			break;
		}

	}

}


void DEngine::Interface()
{
	bool p_open = false;

	ImGuiViewport* MainViewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos({0, 0});
	//ImGui::SetNextWindowSize({ 500, 500 });



	if (!ImGui::Begin("Objects", &p_open, ImGuiWindowFlags_NoTitleBar))
	{
		return;

	}

	if (ImGui::CollapsingHeader("Objects"))
	{
		for (auto &actor : *m_world->GetActors())
		{
			if(ImGui::TreeNode(actor->GetName().c_str()))
			{
				AddInterface(actor->GetProperties());



				ImGui::SeparatorText("Render Properties");
				XMFLOAT3 Position = actor->GetPosition();
				XMFLOAT3 Rotation = actor->GetRotation();
				XMFLOAT3 Scale = actor->GetScale();


				ImGui::DragFloat3("Position", ((float*)&Position), 0.1f);
				ImGui::DragFloat3("Rotation", ((float*)&Rotation), 1.f, 0, 360, "%.1f");
				ImGui::DragFloat3("Scale", ((float*)&Scale));


				ImGui::SeparatorText("Texture Properties");

				auto mat = actor->GetModel()->GetMaterial()->t_Albedo;

				TArray<FString> Items;
				TArray<D3D12Texture*> TexturesPtr;

				int32 CurrentId = -1;
				int32 index = 0;

				auto textures = D3DUtil::GetTextures();
				for (auto& i : *textures)
				{
					if (mat == i.second.get())
						CurrentId = index;
					Items.push_back(i.second->name);
					TexturesPtr.push_back(i.second.get());
					index++;
				}

				if (ImGui::BeginListBox("Textures"))
				{
					for (size_t i = 0; i < Items.size(); i++)
					{
						const bool is_selected = (CurrentId == i);
						if (ImGui::Selectable(Items[i].c_str(), is_selected))
						{
							CurrentId = i;
							if (mat != TexturesPtr[i])
							{
								actor->GetModel()->GetMaterial()->t_Albedo = TexturesPtr[i];
								actor->GetModel()->GetMaterial()->m_BaseDescriptorIndex = -1;
							}
						}
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndListBox();
				}





				actor->SetPosition(Position);
				actor->SetRotation(Rotation);
				actor->SetScale(Scale);

				ImGui::TreePop();

			}

		}

	}

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


	//m_editor->Update(deltaTime);
	Tick(deltaTime);

	FGameTimer::Tick();
	 
}


//void DEngine::SetEditor(DEditor* edt)
//{
//	if (edt != nullptr && GEngine.m_editor == nullptr) 
//	{
//		GEngine.m_editor = edt;
//	}
//}

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
	if (m_renderer)
		m_renderer->Shutdown();
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


