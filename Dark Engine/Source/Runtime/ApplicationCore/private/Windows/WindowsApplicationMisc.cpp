#include "Windows/WindowsApplicationMisc.h"


#include "Runtime/Launch/Resource/Windows/resource.h"





FGenericApplication* FWindowsApplicationMisc::CreateApplication()
{
    FGenericApplication* Result = nullptr;

    HICON AppIconHandle = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));


    Result = FWindowsApplication::CreateWindowsApplication(hInstance, AppIconHandle);

    return Result;
}
