#include "D3D12BarrierBatcher.h"



FD3D12BarrierBatcher::FD3D12BarrierBatcher()
{


}

void FD3D12BarrierBatcher::TransitionResource(FD3D12Buffer* InBuffer, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, uint64 Size, uint64 Offset)
{
	//auto It = std::find(BufferBarriers(InBuffer->GetResouce()));

	if (AfterSync == InBuffer->GetBarrierSync() && AfterAccess == InBuffer->GetBarrierAccess())
	{
		return;
	}


	auto It = std::find_if(BufferBarriers.begin(), BufferBarriers.end(), [InBuffer](const auto& Value)
		{
			return Value.first == InBuffer->GetResouce();
		});
			  
	if (It != BufferBarriers.end() && It->second.Size == Size && It->second.Offset == Offset)
	{
		CD3DX12_BUFFER_BARRIER& Barrier = (*It).second;

		if(AfterAccess == D3D12_BARRIER_ACCESS_NO_ACCESS)
			Barrier.AccessAfter = D3D12_BARRIER_ACCESS_NO_ACCESS;
		else
			Barrier.AccessAfter |= AfterAccess;
		if(AfterSync == D3D12_BARRIER_SYNC_NONE)
			Barrier.SyncAfter = D3D12_BARRIER_SYNC_NONE;
		else
			Barrier.SyncAfter |= AfterSync;
	}
	else
	{
		CD3DX12_BUFFER_BARRIER Barrier;
		//Barrier.BarrierType = D3D12_BARRIER_TYPE_BUFFER;
		Barrier.SyncBefore = InBuffer->GetBarrierSync();
		Barrier.AccessBefore = InBuffer->GetBarrierAccess();
		Barrier.SyncAfter = AfterSync;
		Barrier.AccessAfter = AfterAccess;
		Barrier.Offset = Offset;
		Barrier.Size = Size;
		Barrier.pResource = InBuffer->GetResouce()->GetResource();

		InBuffer->EmptyBarrierFlags();
		BufferBarriers.Emplace(InBuffer->GetResouce(), Barrier);
	}

	InBuffer->SetBarrierAccess(AfterAccess);
	InBuffer->SetBarrierSync(AfterSync);

}

void FD3D12BarrierBatcher::TransitionResource(FD3D12Texture* InTexture, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource)
{
	if (AfterSync == InTexture->GetBarrierSync() && AfterAccess == InTexture->GetBarrierAccess())
	{
		return;
	}


	auto It = std::find_if(TextureBarriers.begin(), TextureBarriers.end(), [InTexture](const auto& Value)
		{
			return Value.first == InTexture->GetResource();
		});

	if (It != TextureBarriers.end())
	{
		CD3DX12_TEXTURE_BARRIER& Barrier = (*It).second;

		if (AfterAccess == D3D12_BARRIER_ACCESS_NO_ACCESS)
			Barrier.AccessAfter = D3D12_BARRIER_ACCESS_NO_ACCESS;
		else
			Barrier.AccessAfter |= AfterAccess;
		if (AfterSync == D3D12_BARRIER_SYNC_NONE)
			Barrier.SyncAfter = D3D12_BARRIER_SYNC_NONE;
		else
			Barrier.SyncAfter |= AfterSync;
		Barrier.LayoutAfter = AfterLayout;
	}
	else
	{
		CD3DX12_TEXTURE_BARRIER Barrier;
		//Barrier.BarrierType = D3D12_BARRIER_TYPE_BUFFER;
		Barrier.LayoutBefore = InTexture->GetBarrierLayout();
		Barrier.SyncBefore = InTexture->GetBarrierSync();
		Barrier.AccessBefore = InTexture->GetBarrierAccess();
		Barrier.SyncAfter = AfterSync;
		Barrier.AccessAfter = AfterAccess;
		Barrier.LayoutAfter = AfterLayout;
		Barrier.pResource = InTexture->GetResource()->GetResource();
		Barrier.Subresources = Subresource;

		InTexture->EmptyBarrierFlags();
		TextureBarriers.Emplace(InTexture->GetResource(), Barrier);
	}

	InTexture->SetBarrierAccess(AfterAccess);
	InTexture->SetBarrierSync(AfterSync);
	InTexture->SetBarrierLayout(AfterLayout);

}

void FD3D12BarrierBatcher::TransitionTexture(FD3D12Resource* InTexture, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource)
{
	if (AfterSync == InTexture->GetBarrierSync() && AfterAccess == InTexture->GetBarrierAccess())
	{
		return;
	}

	auto It = std::find_if(TextureBarriers.begin(), TextureBarriers.end(), [InTexture](const auto& Value)
		{
			return Value.first == InTexture;
		});

	if (It != TextureBarriers.end())
	{
		CD3DX12_TEXTURE_BARRIER& Barrier = (*It).second;

		if (AfterAccess == D3D12_BARRIER_ACCESS_NO_ACCESS)
			Barrier.AccessAfter = D3D12_BARRIER_ACCESS_NO_ACCESS;
		else
			Barrier.AccessAfter |= AfterAccess;
		if (AfterSync == D3D12_BARRIER_SYNC_NONE)
			Barrier.SyncAfter = D3D12_BARRIER_SYNC_NONE;
		else
			Barrier.SyncAfter |= AfterSync;
		Barrier.LayoutAfter = AfterLayout;
	}
	else
	{
		CD3DX12_TEXTURE_BARRIER Barrier;
		//Barrier.BarrierType = D3D12_BARRIER_TYPE_BUFFER;
		Barrier.LayoutBefore = InTexture->BarrierLayout;
		Barrier.SyncBefore = InTexture->GetBarrierSync();
		Barrier.AccessBefore = InTexture->GetBarrierAccess();
		Barrier.SyncAfter = AfterSync;
		Barrier.AccessAfter = AfterAccess;
		Barrier.LayoutAfter = AfterLayout;
		Barrier.pResource = InTexture->GetResource();
		Barrier.Subresources = Subresource;

		InTexture->EmptyBarrierFlags();
		TextureBarriers.Emplace(InTexture, Barrier);
	}

	InTexture->SetBarrierAccess(AfterAccess);
	InTexture->SetBarrierSync(AfterSync);
	InTexture->BarrierLayout = AfterLayout;


}

void FD3D12BarrierBatcher::TransitionBuffer(FD3D12ResourceLocation& Resource, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, uint64 Size, uint64 Offset)
{
	if (AfterSync == Resource.GetBarrierSync() && AfterAccess == Resource.GetBarrierAccess())
	{
		return;
	}


	auto It = std::find_if(BufferBarriers.begin(), BufferBarriers.end(), [Resource](const auto& Value)
		{
			return Value.first == Resource.GetResource();
		});

	if (It != BufferBarriers.end() && It->second.Size == Size && It->second.Offset == Offset)
	{
		CD3DX12_BUFFER_BARRIER& Barrier = (*It).second;

		if (AfterAccess == D3D12_BARRIER_ACCESS_NO_ACCESS)
			Barrier.AccessAfter = D3D12_BARRIER_ACCESS_NO_ACCESS;
		else
			Barrier.AccessAfter |= AfterAccess;
		if (AfterSync == D3D12_BARRIER_SYNC_NONE)
			Barrier.SyncAfter = D3D12_BARRIER_SYNC_NONE;
		else
			Barrier.SyncAfter |= AfterSync;
	}
	else
	{
		CD3DX12_BUFFER_BARRIER Barrier;
		//Barrier.BarrierType = D3D12_BARRIER_TYPE_BUFFER;
		Barrier.SyncBefore = Resource.GetBarrierSync();
		Barrier.AccessBefore = Resource.GetBarrierAccess();
		Barrier.SyncAfter = AfterSync;
		Barrier.AccessAfter = AfterAccess;
		Barrier.Offset = Offset;
		Barrier.Size = Size;
		Barrier.pResource = Resource.GetResource()->GetResource();

		Resource.EmptyBarrierFlags();
		BufferBarriers.Emplace(Resource.GetResource(), Barrier);
	}

	Resource.SetBarrierAccess(AfterAccess);
	Resource.SetBarrierSync(AfterSync);

}

void FD3D12BarrierBatcher::FlushBarrierGroups(FD3D12CommandList& CmdList)
{
	if(BufferBarriers.Num() || TextureBarriers.Num())
	{
		static TArray<CD3DX12_BARRIER_GROUP> BarrierGroups(ArrayReserve(3));
		static std::vector<CD3DX12_BUFFER_BARRIER>	BufBarriers;
		static std::vector<CD3DX12_TEXTURE_BARRIER> TexBarriers;
		BarrierGroups.Empty();
		BufBarriers.clear();
		TexBarriers.clear();

		BufBarriers.reserve(BufferBarriers.Num());
		TexBarriers.reserve(TextureBarriers.Num());


		std::transform(BufferBarriers.begin(), BufferBarriers.end(), std::back_inserter(BufBarriers), [](const auto& Value)->auto
			{
				return Value.second;
			});

		std::transform(TextureBarriers.begin(), TextureBarriers.end(), std::back_inserter(TexBarriers), [](const auto& Value)->auto
			{
				return Value.second;
			});

		if (BufBarriers.size())
			BarrierGroups.Add(CD3DX12_BARRIER_GROUP(BufBarriers.size(), BufBarriers.data()));
		if (TexBarriers.size())
			BarrierGroups.Add(CD3DX12_BARRIER_GROUP(TexBarriers.size(), TexBarriers.data()));

		if (BarrierGroups.Num())
			CmdList.GetGraphicsCommandList7()->Barrier(BarrierGroups.Num(), BarrierGroups.GetData());

		BufferBarriers.Empty();
		TextureBarriers.Empty();
	}
}
