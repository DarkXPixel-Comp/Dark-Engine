#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "Memory/MemoryCore.h"
#include "Containers/DarkString.h"

struct FMonitorInfo
{
	FString Name;
	FString ID;
	int32 NativeWidth;
	int32 NativeHeight;
	
	bool IsPrimary;
	int32 DPI;

};


struct FDisplayMetrics
{
	FDisplayMetrics()
	{

	}

	int32 PrimaryDisplayWidth;
};


class FGenericApplication
{
public:
	virtual ~FGenericApplication() {}

	virtual TSharedPtr<FGenericWindow> MakeWindow()
	{
		return MakeShareble(new FGenericWindow());
	}

	virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindow, FGenericWindowDefinition InDefinition, const TSharedPtr<FGenericWindow>& InParent, bool bShow = false) {}
	virtual void PumpMessages() {}
	virtual void Tick(float DeltaTime) {}
	virtual void SetMessageHandler(const TSharedPtr<FGenericApplicationMessageHandler>& InMessageHandler) { MessageHandler = InMessageHandler; }

protected:
	TSharedPtr<FGenericApplicationMessageHandler> MessageHandler;
};