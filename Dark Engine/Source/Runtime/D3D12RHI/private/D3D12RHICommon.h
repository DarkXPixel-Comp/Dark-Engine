#pragma once
#include "D3D12RHI.h"
#include "D3D12ThirdParty.h"


#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY(D3D12RHI, Log);




class FD3D12Adapter;
class FD3D12Device;



class FD3D12AdapterChild
{
protected:
	FD3D12Adapter* Parent;

public:
	FD3D12AdapterChild(FD3D12Adapter* InParent) : Parent(InParent) {}

	FD3D12Adapter* GetParentAdapter() const { return Parent; }

	void SetParentAdapter(FD3D12Adapter* InParent) { Parent = InParent; }


};



class FD3D12DeviceChild
{
protected:
	FD3D12Device* Parent;

public:
	FD3D12DeviceChild(FD3D12Device* InParent) : Parent(InParent) {}
	FD3D12Device* GetParentDevice() const { return Parent; }

	void SetParentDevice(FD3D12Device* InParent) { Parent = InParent; }

};
