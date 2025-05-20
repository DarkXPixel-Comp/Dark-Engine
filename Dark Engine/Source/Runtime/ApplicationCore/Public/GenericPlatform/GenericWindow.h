#pragma once
#include "CoreTypes.h"
#include "Memory/MemoryCore.h"
#include "GenericPlatform/GenericWindowDefinition.h"


class APPLICATIONCORE_API FGenericWindow
{
public:
	FGenericWindow() {}

	virtual bool GetFullScreenInfo(int32& X, int32& Y, int32& Width, int32& Height) const { return false; }

	virtual void Destroy() {}

	virtual void Show() {}

	virtual void Update() {}

	virtual void Maximize() {}

	virtual void Minimize() {}

	virtual TSharedPtr<FGenericWindow> MakeWindow() { return MakeShareble(new FGenericWindow()); }

	virtual EWindowMode GetWindowMode() const { return EWindowMode::Windowed; }

	virtual void* GetOSWindowHandle() const { return nullptr; }

	virtual void AdjustSize(FVector2f& Size) const {}

	//virtual void SetTitle(const FString& InTitle) {}
};