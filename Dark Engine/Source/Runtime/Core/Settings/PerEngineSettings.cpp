#include "PerEngineSettings.h"

PerEngineSettings* PerEngineSettings::instance;

PerEngineSettings settings;

PerEngineSettings::PerEngineSettings()
{
	instance = this;

	wcscpy_s(m_ProjectName, L"undefined");
	wcscpy_s(m_ShortName, L"undefined");
	m_MainIcon = LoadIcon(NULL, IDI_APPLICATION);
}



