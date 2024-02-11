#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"


class FGenericApplication
{
public:
	virtual TSharedPtr<FGenericWindow> MakeWindow()
	{
		return MakeShareble(new FGenericWindow());
	}
	virtual void InitializeWindow(const TSharedPtr<FGenericWindow>& InWindows, const TSharedPtr<FGenericWindowDefinition>& InDefinition) {}


};