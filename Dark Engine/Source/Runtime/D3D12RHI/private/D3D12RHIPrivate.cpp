#include "D3D12RHIPrivate.h"
#include <Misc/AssertionMacros.h>


FD3D12DynamicRHI* FD3D12DynamicRHI::SingleD3D12RHI = nullptr;

FD3D12DynamicRHI::FD3D12DynamicRHI(const TArray<TSharedPtr<FD3D12Adapter>>& InChosenAdapters)
	: ChosenAdapters(InChosenAdapters)
{
	if (SingleD3D12RHI)
	{
		return;
	}

	SingleD3D12RHI = this;

	FeatureLevel = GetAdapter().GetFeatureLevel();




}

FD3D12DynamicRHI::~FD3D12DynamicRHI()
{
}

void FD3D12DynamicRHI::Init()
{
	for (auto& Adapter : ChosenAdapters)
	{
		check(Adapter->GetDesc().IsValid());
		Adapter->InitializeDevices();
	}


}