#pragma once
#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"


class FDUIApplicationBase
{
	friend class DUIWidget;
public:
	DarkUI_API FDUIApplicationBase();
	virtual ~FDUIApplicationBase() {}

	virtual bool IsActive() const = 0;


protected:
	TSharedPtr<class FDUIRenderer> Renderer;

	static DarkUI_API TSharedPtr<FDUIApplicationBase> CurrentBaseApplication;
	static DarkUI_API TSharedPtr<class FGenericApplication>	PlatformApplication;

	//FDisplayMetrics DisplayMetrics;




};