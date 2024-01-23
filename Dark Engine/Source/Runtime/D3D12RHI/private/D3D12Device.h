#pragma once
#include "CoreMinimal.h"
#include "D3D12RHI.h"

#include "D3D12Adapter.h"

#include "D3D12RHICommon.h"


class FD3D12Device : public FD3D12AdapterChild
{
public:
	FD3D12Device(FD3D12Adapter* InAdapter);

	ID3D12Device* GetDevice();




private:
	//FD3D12Adapter* ParentAdapter;

};