#pragma once

#include "D3D12Defines.h"
#include "HAL/Platform.h"
#include "D3D12ThirdParty.h"
#include "D3D12DynamicRHI.h"
#include "Memory/TUniquePtr.h"
#include "Containers/Array.h"


class FD3D12Adapter;
class FD3D12Device;



class FD3D12DynamicRHIModule : public IDynamicRHIModule
{
public:
	FD3D12DynamicRHIModule();

	virtual FDynamicRHI* CreateRHI() override;


private:
	void FindAdapter();
	TArray<TSharedPtr<FD3D12Adapter>> ChosenAdapters;



};