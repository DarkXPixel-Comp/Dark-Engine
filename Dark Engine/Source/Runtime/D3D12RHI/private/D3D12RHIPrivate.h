#pragma once
#include "D3D12RHI.h"
#include "D3D12Adapter.h"




class FD3D12DynamicRHI :public ID3D12PlatformDynamicRHI
{
	static FD3D12DynamicRHI* SingleD3D12RHI;

public:
	static FD3D12DynamicRHI* GetD3DRHI12() { return SingleD3D12RHI; }

	FD3D12DynamicRHI(const TArray<TSharedPtr<FD3D12Adapter>>& InChosenAdapters);

	~FD3D12DynamicRHI();

	FD3D12Adapter& GetAdapter(int32 Index = 0) { return *ChosenAdapters[Index]; }
	const FD3D12Adapter& GetAdapter(int32 Index = 0) const { return *ChosenAdapters[Index]; }

	const uint32 GetNumAdapters() const { return ChosenAdapters.size(); }

	void Init() override;

	virtual void PostInit() {}
	virtual void Shutdown() {}


private:
	TArray<TSharedPtr<FD3D12Adapter>> ChosenAdapters;
	D3D_FEATURE_LEVEL FeatureLevel;




};

