#pragma once
#include "RHIDefines.h"
#include "CoreTypes.h"
#include "Containers/DarkString.h"
#include "Containers/Array.h"



struct FRHIViewDesc
{
	enum class EViewType
	{
		BufferSRV,
		BufferUAV,
		TextureUAV,
		TextureSRV
	};

	enum class EBufferType
	{

	};

	enum class EDimension
	{
		Unknown,
		Texture2D,
		TextureCube,
		Texture3D
	};
};



class FRHIResource
{
	friend class FRHICommandListImmediate;

public:
	FRHIResource(ERHIResourceType InResourceType): ResourceType(InResourceType) {}
	virtual ~FRHIResource() {}

	uint32 Release() { return 0; }


	bool IsValid() const
	{
		return bIsValid;
	}
	void SetValid(bool InIsValid) { bIsValid = InIsValid; }

	void Delete()
	{
		delete this;
	}

	ERHIResourceType GetResourceType() const { return ResourceType; }

	const FString& GetName() const { return Name; }




private:
	void Destroy();

private:
	bool bIsValid = true;




protected:
	ERHIResourceType ResourceType;
	FString Name;

};


class FRHIViewableResource : public FRHIResource
{
public:
	FRHIViewableResource(ERHIResourceType InResourceType, ERHIAcces InAccess):
		FRHIResource(InResourceType),
		Access(InAccess)
	{}

private:
	ERHIAcces Access;

};

class FRHIView : public FRHIResource
{
public:
	FRHIView(ERHIResourceType InResourceType, FRHIViewableResource* InResource, FRHIViewDesc const& InViewDesc) :
		FRHIResource(InResourceType),
		Resource(InResource),
		ViewDesc(InViewDesc)
	{}



private:
	FRHIViewableResource* Resource;
	FRHIViewDesc ViewDesc;
};

class FRHIGraphicsPipelineState : public FRHIResource
{
public:
	FRHIGraphicsPipelineState() : FRHIResource(RRT_GraphicsPipelineState) {}

};

class FRHIComputePipelineState : public FRHIResource
{
public:
	FRHIComputePipelineState() : FRHIResource(RRT_ComputePipelineState) {}


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

	uint32 GetSize() const { return Desc.Size; }
	uint32 GetStride() const { return Desc.Stride; }
	const FRHIBufferDesc& GetDesc() { return Desc; }


	FString GetName() const { return Name; }
	void SetName(FString InName) { Name = InName; }

	



private:
	FRHIBufferDesc Desc = {};

};



class FRHITexture : public FRHIResource
{

};


class FRHIViewport : public FRHIResource
{
public:
	FRHIViewport() : FRHIResource(RRT_Viewport)  {}

	virtual void* GetNativeSwapChain() const { return nullptr; }

	virtual void Tick(float DeltaTime) {}
	virtual void WaitForFrameEventCompletion() {}

};


class FRHIShader : public FRHIResource
{
public:
	struct
	{
		FString ShaderName;
		TArray<FString>	UniformBufferNames;
	} Debug;
	const TCHAR* GetShaderName() const
	{
		return Debug.ShaderName.Len() ? *Debug.ShaderName : TEXT("UNKNOWN");
	}
	FString GetUniformBufferName(uint32 Index) const
	{
		return Debug.UniformBufferNames.IsValidIndex(Index) ? Debug.UniformBufferNames[Index] : TEXT("UNKNOWN");
	}


	EShaderType GetType() const { return Type;}
	FRHIShader(ERHIResourceType InResourceType, EShaderType InType) :
		FRHIResource(InResourceType),
		Type(InType)
	{}


private:
	struct ShaderID
	{
		uint64 ShaderID;
		static uint64 NextShaderID;
	};

	EShaderType Type;
};

class FRHIGraphicsShader : public FRHIShader
{
public:
	FRHIGraphicsShader(ERHIResourceType InResource, EShaderType InType) :
		FRHIShader(InResource, InType)
	{}

};

class FRHIVertexShader : public FRHIGraphicsShader
{
public:
	FRHIVertexShader() : FRHIGraphicsShader(RRT_VertexShader, ST_Vertex)
	{}
};
class FRHIPixelShader : public FRHIGraphicsShader
{
public:
	FRHIPixelShader() : FRHIGraphicsShader(RRT_PixelShader, ST_Pixel)
	{}
};


class FRHISamplerState : public FRHIResource
{
	FRHISamplerState() : FRHIResource(RRT_SamplerState) {}
	virtual FRHIDescriptorHandle GetBindlessHandle() const { return FRHIDescriptorHandle(); }
};