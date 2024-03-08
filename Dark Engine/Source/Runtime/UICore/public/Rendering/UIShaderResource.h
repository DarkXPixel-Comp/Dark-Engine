#pragma once
#include "CoreTypes.h"
#include "RHIResources.h"


enum EUIShaderResouce
{
	NativeTexture,
	Material,
	PostProcess,
	Null
};





class FUIShaderResource
{
public:
	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;

	virtual EUIShaderResouce GetType() const = 0;

	virtual bool IsResourceValid() const = 0;

};


//class FUIRenderTarget : public FUIShaderResource
//{
//public:
//	virtual uint32 GetWidth() const { return NativeTexture->GetSizeX(); }
//	virtual uint32 GetHeight() const { return NativeTexture->GetSizeY(); }
//
//	virtual EUIShaderResouce GetType() const { return EUIShaderResouce::NativeTexture; }
//
//	virtual bool IsResourceValid() const {}
//
//
//
//
//private:
//	TSharedPtr<FRHITexture> NativeTexture;
//
//
//
//
//
//};