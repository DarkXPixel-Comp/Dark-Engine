#pragma once
#include "CoreTypes.h"


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