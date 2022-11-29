#pragma once
#include "../CoreMinimal.h"



#ifdef _WIN32



class PerEngineSettings
{
private:
	static PerEngineSettings* instance;
	
public:
	static PerEngineSettings* Instance() { return instance; }

public:
	PerEngineSettings(); // CSTR
	//~PerEngineSettings(); // DSTR

private:
	WCHAR m_ProjectName[MAX_NAME_STRING];
	WCHAR m_ShortName[MAX_NAME_STRING];
	HICON m_MainIcon;
	WCHAR m_BootTime[MAX_NAME_STRING];
	WCHAR m_SplashURL[MAX_NAME_STRING];

public:
	const static WCHAR* ProjectName() { return instance->m_ProjectName; }
	static VOID SetProjectName(UINT id) {
		LoadStringW(GetModuleHandleW(NULL), id, instance->m_ProjectName, MAX_NAME_STRING);
	}

	static WCHAR* ShortName() { return instance->m_ShortName; }
	static VOID SetShortName(UINT id) { LoadStringW(GetModuleHandleW(NULL), id, instance->m_ShortName, MAX_NAME_STRING); }

	static HICON MainIcon() { return instance->m_MainIcon; }
	static VOID SetMainIcon(UINT id) { LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(id)); }

	static WCHAR* BootTime() { return instance->m_BootTime; }

	static WCHAR* SplashURL() { return instance->m_SplashURL; }
	static VOID SetSplashURL(UINT id) { LoadStringW(GetModuleHandleW(NULL), id, instance->m_SplashURL, MAX_NAME_STRING); }


};


#endif // _WIN32