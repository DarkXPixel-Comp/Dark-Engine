#include "Windows/WindowsApplicationMisc.h"


#include "Runtime/Launch/Resource/Windows/resource.h"
#include "Misc/Paths.h"





FGenericApplication* FWindowsApplicationMisc::CreateApplication()
{
    FGenericApplication* Result = nullptr;

    HICON AppIconHandle = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    HCURSOR AppCursorHandle = LoadCursorFromFile(*FString::PrintF(TEXT("%sCursors/DarkCursors/Normal Select.cur"), *FPaths::EngineContentDir()));


    Result = FWindowsApplication::CreateWindowsApplication(hInstance, AppIconHandle, AppCursorHandle);

    return Result;
}
