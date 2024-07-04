#include "Widgets/UIProfiler.h"
#include "imgui.h"
#include "optick.h"
#include "Templates/DarkTemplate.h"
#include "Misc/Paths.h"
#include "imgui_internal.h"
#include "Misc/ProcessManager.h"


UIProfiler::UIProfiler() : UIWidget(TEXT("UIProfiler")), Buffer("DefaultCapture.opt")
{
	bHaveCloseButton = true;
}

UIProfiler::~UIProfiler() noexcept
{
	StopAndSaveCapture();
}

void UIProfiler::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

void UIProfiler::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose()))
	{
		ImGui::InputText("Output file name with format", Buffer, DE_ARRAY_COUNT(Buffer));
		bool PushFlag = false;
		if (bInCapture)
		{
			PushFlag = true;
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::Button("Start capture", ImVec2(0, 0)))
		{
			bInCapture = true;
			OPTICK_START_CAPTURE(Optick::Mode::DEFAULT, 1000, false);
		}
		if (PushFlag)
		{
			PushFlag = false;
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		if (!bInCapture)
		{
			PushFlag = true;
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::Button("Stop and save capture", ImVec2(0, 0)))
		{
			StopAndSaveCapture();
		}
		if (PushFlag)
		{
			PushFlag = false;
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}


		if (ProfilerProcess && ProfilerProcess->IsValid())
		{
			PushFlag = true;
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::Button("Open profiler"))
		{
			OpenProfiler();
		}
		if (PushFlag)
		{
			PushFlag = false;
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

	}
	ImGui::End();
}

void UIProfiler::OpenProfiler()
{
	/*SInfo.cb = sizeof(SInfo);

	CreateProcess(NULL, *FString(FString::PrintF(TEXT("ThirdParty/Optick/Optick.exe ProfilerCaptures/%s"), *FString(Buffer))),
		NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &SInfo, &PInfo);*/

	ProfilerProcess = FProcessManager::NewProcess(TEXT("ThirdParty/Optick/Optick.exe"), *FString::PrintF(TEXT("ProfilerCaptures/%s"), *FString(Buffer)));
}

void UIProfiler::StopAndSaveCapture()
{
	if(bInCapture)
	{
		bInCapture = false;
		OPTICK_STOP_CAPTURE(false);
		OPTICK_SAVE_CAPTURE(-FString::PrintF(TEXT("%sProfilerCaptures/%s"), *FPaths::EngineBinariesDir(), *FString(Buffer)));
	}
}
