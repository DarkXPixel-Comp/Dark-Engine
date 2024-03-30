#include "D3D12StateCache.h"

FD3D12StateCache::FD3D12StateCache(FD3D12CommandContext& CmdContext):
	FD3D12DeviceChild(CmdContext.GetParentDevice()),
	Context(CmdContext)
{




}
