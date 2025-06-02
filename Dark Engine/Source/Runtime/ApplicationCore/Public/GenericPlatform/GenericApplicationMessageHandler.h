#pragma once

#include "Platform/Platform.h"
#include "Memory/MemoryCore.h"
#include "GenericPlatform/GenericWindow.h"


class FGenericApplicationMessageHandler
{
public:
	virtual ~FGenericApplicationMessageHandler() {}

	virtual bool ShouldProcessUserInputMessages(const TSharedPtr<FGenericWindow>& PlatformWindow) const
	{
		return false;
	}

	virtual EWindowZone GetWindowZoneForPoint(const TSharedPtr<FGenericWindow>& Window, const int32 X, const int32 Y)
	{
		return EWindowZone::NotInWindow;
	}

	virtual bool OnWindowActivationChanged(const TSharedPtr<FGenericWindow>& Window, const EWindowActivation ActivationType)
	{
		return false;
	}

	virtual bool OnApplicationActivationChanged(bool bIsActive)
	{
		return false;
	}

	virtual void OnOSPaint(const TSharedPtr<FGenericWindow>& Window)
	{

	}

	virtual void OnWindowClose(const TSharedPtr<FGenericWindow>& Window)
	{

	}

	virtual bool OnWindowAction(const TSharedPtr<FGenericWindow>& Window, EWindowAction InActionType)
	{
		return true;
	}

	virtual FWindowSizeLimits GetSizeLimitsForWindow(const TSharedPtr<FGenericWindow>& Window) const
	{
		return FWindowSizeLimits();
	}

};