#include "Application/DApplication.h"
#include "Widgets/DWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Platform/PlatformApplicationMisc.h"

TSharedPtr<class DUIWindow> FDUIApplication::AddWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow)
{

	

}

void FDUIApplication::InitHightDPI(bool bForceEnable)
{
	FPlatformApplicationMisc::SetHighDPIMode();



}

TSharedPtr<FGenericWindow> FDUIApplication::MakeWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow)
{
	TSharedPtr<FGenericWindow> NativeParent = nullptr;
	TSharedPtr<DUIWindow> ParentWindow = nullptr;

	FGenericWindowDefinition Definition;

	Definition.Type = InWindow->GetType();




}
