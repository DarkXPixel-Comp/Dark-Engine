#pragma once

#include "../Core/CoreMinimal.h"



struct FCrashReportCoreConfig
{
private:
	static FCrashReportCoreConfig* inst;

public:
	FCrashReportCoreConfig();


	static FCrashReportCoreConfig* getHandle();





	
};
