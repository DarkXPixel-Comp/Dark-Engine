#pragma once
#include "D3D12Resources.h"
#include <d3d12.h>


void VerifyD3D12Result(HRESULT hr, const ANSICHAR* Code, const ANSICHAR* Filename, uint32 Line)
{
	FString Error = Code;
}




#define DXCall(x) {HRESULT Result = x; if(FAILED(Result)) {VerifyD3D12Result(Result, #x, __FILE__, __LINE__);}}



void SetName(ID3D12Object* const Object, const TCHAR* const Name)
{
	if (Object && Name)
	{
		Object->SetName(Name);
		return;
	}
	Object->SetName(L"NULL");



}

void SetName(FD3D12Resource* const Resource, const TCHAR* const Name)
{

}

