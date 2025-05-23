#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "Memory/MemoryCore.h"
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "Math/IntPoint.h"
#include "Math/Rect.h"

struct FMonitorInfo
{
	FString Name;
	FString ID;
	int32 NativeWidth = 0;
	int32 NativeHeight = 0;
	FIntPoint MaxResolution;
	FIntRect DisplayRect;
	FIntRect WorkAreaRect;
	bool bIsPrimary;
	int32 DPI = 0;

};


struct FDisplayMetrics
{
	FDisplayMetrics()
	{

	}

	int32 PrimaryDisplayWidth;
	int32 PrimaryDisplayHeight;

	TArray<FMonitorInfo> MonitorInfo;

	FIntRect PrimaryDisplayWorkAreaRect;
	FIntRect VirtualDisplayRect;
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
	virtual void PumpMessages(float DeltaTime) {}
	virtual void Tick(float DeltaTime) {}
	virtual void SetMessageHandler(const TSharedPtr<FGenericApplicationMessageHandler>& InMessageHandler) { MessageHandler = InMessageHandler; }

	virtual void RebuildDisplayMetrics(FDisplayMetrics& OutDisplay) {}
protected:
	TSharedPtr<FGenericApplicationMessageHandler> MessageHandler;
};