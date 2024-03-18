#pragma once
#include "D3D12Resources.h"
#include <d3d12.h>


FORCEINLINE void VerifyD3D12Result(HRESULT hr, const ANSICHAR* Code, const ANSICHAR* Filename, uint32 Line)
{
	FString Error = Code;
	DE_LOG(D3D12RHI, Error, TEXT("%i = %s \t in %s(%i)"), hr, *Error, *FString(Filename), Line);
}




#define DXCall(x) {HRESULT Result = x; if(FAILED(Result)) {VerifyD3D12Result(Result, #x, __FILE__, __LINE__);}}



FORCEINLINE void SetName(ID3D12Object* const Object, const TCHAR* const Name)
{
	if (Object && Name)
	{
		Object->SetName(Name);
		return;
	}
	Object->SetName(L"NULL");



}

FORCEINLINE void SetName(FD3D12Resource* const Resource, const TCHAR* const Name)
{

}

namespace DX12
{
	FORCEINLINE void SetName(ID3D12Object* const Object, const TCHAR* const Name)
	{
		if (Object && Name)
		{
			Object->SetName(Name);
			return;
		}
		Object->SetName(L"NULL");



	}

	FORCEINLINE void SetName(FD3D12Resource* const Resource, const TCHAR* const Name)
	{

	}

}

