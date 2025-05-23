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

	virtual bool IsMaximize() const { return false; }

	virtual bool IsMinimize() const { return false; }

	virtual TSharedPtr<FGenericWindow> MakeWindow() { return MakeShareble(new FGenericWindow()); }

	virtual float GetDPIScale() const{ return 1.0f; }

	virtual void SetDPIScale(float) {  }

	virtual EWindowMode GetWindowMode() const { return EWindowMode::Windowed; }

	virtual void* GetOSWindowHandle() const { return nullptr; }

	virtual void AdjustSize(FVector2d& Size) const {}

	virtual bool IsPointInWindow(int32 X, int32 Y) const { return false; }

	//virtual void SetTitle(const FString& InTitle) {}
};