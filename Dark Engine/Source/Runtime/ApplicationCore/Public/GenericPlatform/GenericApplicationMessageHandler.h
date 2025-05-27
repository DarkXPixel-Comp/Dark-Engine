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



};