#pragma once

#include "RenderResource.h"



extern TArray<FRenderResource*>& GetInitializingRenderResources();

template<class T>
class TGlobalRenderResource
{
public:
	TGlobalRenderResource()
	{
		Resource = new T();	   
		GetInitArray().Add(Resource);
	}

	bool IsValid() const
	{
		Resource != nullptr;
	}

	static TArray<FRenderResource*>& GetInitArray()
	{
		return GetInitializingRenderResources();
	}

	T* operator*() const
	{
		return GetResource();
	}

	T* operator->() const
	{
		return GetResource();
	}

	T* GetResource() const
	{
		return static_cast<T*>(Resource);
	}

private:
	FRenderResource* Resource = nullptr;
};



void InitGlobalRenderResources();



