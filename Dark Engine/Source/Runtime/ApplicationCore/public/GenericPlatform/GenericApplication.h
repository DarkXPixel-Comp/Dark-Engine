#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "CoreMinimal.h"
#include "Math/MathFwd.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericApplicationHandler.h"


class FGenericApplication
{
public:
	virtual TSharedPtr<FGenericWindow> MakeWindow()
	{
		return MakeShareble(new FGenericWindow());
	}
	virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindows, const TSharedPtr<FGenericWindowDefinition>& InDefinition) {}
	virtual void PumpMessages() {}
	virtual void Tick(float DeltaTime) {}
	virtual void SetMessageHandler(const TSharedPtr<FGenericApplicationHandler>& InMessageHandler) { MessageHandler = InMessageHandler; }
	virtual FIntPoint GetMousePos() const { return FIntPoint(); }
	virtual void SetCapture(const TSharedPtr<FGenericWindow>& InWindow) {}



protected:
	TSharedPtr<FGenericApplicationHandler> MessageHandler;
};