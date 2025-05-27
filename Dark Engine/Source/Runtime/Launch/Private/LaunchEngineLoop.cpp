#include "LaunchEngineLoop.h"
#include "Misc/Paths.h"
#include "Templates/DarkTemplate.h"
#include "Platform/PlatformMisc.h"
#include "Console/BaseConsole.h"
#include "CoreGlobals.h"
#include <vector>
#include <filesystem>
#include <fstream>
#include "optick.h"
#include "Misc/Attribute.h"
#include "Widgets/DWindow.h"
#include "Misc/Delegate.h"
#include "physfs.h"
#include "Modules/ModuleManager.h"
#include "Windows/WindowsPlatformProcess.h"
#include "Logging/LogMacros.h"
#include "Misc/AssertionMacros.h"
#include "Misc/OutputDeviceDebug.h"
#include "Misc/GlobalLogger.h"
#include "Windows/WindowsConsoleOutputDevice.h"
#include "Application/DApplication.h"
#include "GenericPlatform/GenericApplicationMisc.h"
//#include "Widgets/DWindow.h"


//import Array;



DECLARE_LOG_CATEGORY(LaunchLog, Log)

static const FString VitalFolders[] = { "Content", "Config", "Shaders", "Binaires" };


static int32 CheckFiles(FString& AdditionalErrorMsg)
{
	int32 AllFilesIsOk = false;
	int32 NumFiles = 0;

	std::vector<FString> TempFolders(std::begin(VitalFolders), std::end(VitalFolders));

	for (const auto& Entry : std::filesystem::directory_iterator(*FPaths::EngineDir()))
	{
		auto& Path = Entry.path();
		auto str = Path.filename().wstring();

		auto Result = std::find(std::begin(VitalFolders), std::end(VitalFolders), Path.filename().wstring());

		if (Result != std::end(VitalFolders))
		{
			++NumFiles;
			TempFolders.erase(std::find(TempFolders.begin(), TempFolders.end(), *Result));
		}
	}


	for (auto& LostFile : TempFolders)
	{
		AdditionalErrorMsg += (LostFile + TEXT("\n"));
	}


	return NumFiles == DE_ARRAY_COUNT(VitalFolders);
}

void CreateMainFrame()
{
	TSharedPtr<DUIWindow> RootWindow = DUINew(DUIWindow)
		.Title(TEXT("HUY"));
	



}

int32 FEngineLoop::PreInit(const FString& CmdLine)
{
	OPTICK_EVENT("PreInit");

	FString ErrorMsg;

	//DUIWindow wnd;
	
	FGlobalLogger::Get().AddOutputDevice(FOutputDeviceDebug::Get());


	//IModule* h = FModuleManager::Get().GetOrLoadModule(TEXT("TestModule"));

	if (!CheckFiles(ErrorMsg))
	{
		FPlatformMisc::CreateMessageBoxError(*FString::PrintF(TEXT("Not found all vital files or folders: \n%s"), *ErrorMsg), TEXT("Error vital files"));
		RequestExit();
		return -1;
	}

	FWindowsConsoleOutputDevice* Console = new FWindowsConsoleOutputDevice();
	Console->Show(true);

	FDUIApplication::InitHightDPI(true);
	FDUIApplication::Create();

	auto wnd = DUINew(DUIWindow)
		.Visibility(EVisibility::All)
		.ScreenPosition({ 0, 0 })
		.Size({800, 600})
		.bHasOSBorder(true)
		.bIsRegularWindow(true);

	FDUIApplication::Get().AddWindow(wnd);

	FDUIApplication::Get().AddWindow(
		DUINew(DUIWindow)
		.Visibility(EVisibility::All)
		.Size({ 500, 500 })
		.bHasOSBorder(true)
		.bIsRegularWindow(true)
		.ScreenPosition({ 800, 600 })
	);

	/*TSharedPtr<DUIWindow> testWnd =  DUINew(DUIWindow)
		.Title(TEXT("TEST"));*/


	/*int* p = nullptr;
	*p = 44;*/
	check(false);

	return 0;
}


int32 FEngineLoop::Init()
{
	return 0;
}

void FEngineLoop::Tick()
{
	//DE_LOG(LaunchLog, Warning, TEXT("HUYHYHFSDSF"));
	FDUIApplication::Get().Tick();
}

void FEngineLoop::Exit()
{
	bExit = true;
	FDUIApplication::Shutdown();
}
