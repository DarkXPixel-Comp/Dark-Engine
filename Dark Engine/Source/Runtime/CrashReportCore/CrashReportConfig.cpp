#include "CrashReportConfig.h"



FCrashReportCoreConfig* FCrashReportCoreConfig::inst;


FCrashReportCoreConfig::FCrashReportCoreConfig()
{
	inst = this;


}


FCrashReportCoreConfig* FCrashReportCoreConfig::getHandle()
{
	return inst;
}