#pragma once
#include "D3D12Resources.h"
#include <d3d12.h>
#include <comdef.h>

FORCEINLINE FString GetDX12ErrorMessage(HRESULT hr)
{
	return _com_error(hr).ErrorMessage();
}


FORCEINLINE FString ParseDX12Error(HRESULT hr)
{
	_com_error error(hr);
	return FString(error.ErrorMessage());
}


FORCEINLINE void VerifyD3D12Result(HRESULT hr, const ANSICHAR* Code, const ANSICHAR* Filename, uint32 Line)
{
	FString Error = Code;
	DE_LOG(D3D12RHI, Error, TEXT("%s = %s \t in %s(%i)"), *GetDX12ErrorMessage(hr), *Error, *FString(Filename), Line);
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

