#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "Memory/MemoryCore.h"



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