#pragma once

#include "CoreTypes.h"
#include "Containers/DarkString.h"


class FGenericWindow
{
public:
	FGenericWindow();

	virtual bool GetFullScreenInfo(int32& X, int32& Y, int32& Width, int32& Height);

	virtual void Destroy();

	virtual void Show();

	virtual void* GetOSWindowHandle() const;

	virtual void SetTitle(FString Title);



	



};