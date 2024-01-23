#pragma once
#include "RHIDefines.h"
#include "CoreTypes.h"
#include "Containers/DarkString.h"



class FRHIResource
{
	friend class FRHICommandListImmediate;

public:
	FRHIResource(ERHIResourceType InResourceType);
	virtual ~FRHIResource();

	uint32 Release();


	bool IsValid() const
	{
		return true;
	}

	void Delete()
	{
		delete this;
	}

	ERHIResourceType GetResourceType() const { return ResourceType; }


public:


private:
	void Destroy();




protected:
	ERHIResourceType ResourceType;
	FString Name;

};


struct FRHIBufferDesc
{
	uint32 Size;
	uint32 Stride;

	FRHIBufferDesc() = default;
	FRHIBufferDesc(uint32 InSize, uint32 InStride) :
		Size(InSize),
		Stride(InStride) 
	{}

	static FRHIBufferDesc Null() { return FRHIBufferDesc(0, 0); }
	
	bool IsNum() const
	{
		if (Size == 0 && Stride == 0)
		{
			return true;
		}
		return false;
	}




};





class FRHIBuffer : public FRHIResource
{
public:
	FRHIBuffer(FRHIBufferDesc& InDesc) :
		FRHIResource(RRT_Buffer),
		Desc(InDesc)	 
	{}

	uint32 GetSize() { return Desc.Size(); }
	uint32 GetStride() { return Desc.Stride(); }
	const FRHIBufferDesc& GetDesc() { return Desc; }


	FString GetName() const { return Name; }
	void SetName(FString InName) { Name = InName; }

	



private:
	FRHIBufferDesc Desc = {};

};



class FRHITexture : public FRHIResource
{

};