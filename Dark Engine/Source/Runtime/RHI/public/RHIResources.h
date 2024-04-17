#pragma once
#include "RHIDefines.h"
#include "Containers/DarkString.h"
#include "Containers/Array.h"
#include "Containers/StaticArray.h"
#include "Misc/AssertionMacros.h"
#include "Math/MathFwd.h"
#include "Templates/RefCounting.h"



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



class FRHIResource : public FRefCountedObject
{
	friend class FRHICommandListImmediate;

public:
	FRHIResource(ERHIResourceType InResourceType): ResourceType(InResourceType) {}
	virtual ~FRHIResource() {}

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

	void SetName(FString InName) { Name = InName; }




private:
	void Destroy();

private:
	bool bIsValid = true;




protected:
	ERHIResourceType ResourceType;
	FString Name = TEXT("RHIResource");

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
	FRHIBuffer(const FRHIBufferDesc& InDesc) :
		FRHIResource(RRT_Buffer),
		Desc(InDesc)	 
	{}

	uint32 GetSize() const { return Desc.Size; }
	uint32 GetStride() const { return Desc.Stride; }
	const FRHIBufferDesc& GetDesc() { return Desc; }



	



private:
	FRHIBufferDesc Desc = {};

};



struct FRHITextureDesc
{
	FRHITextureDesc() = default;
	FRHITextureDesc(const FRHITextureDesc& Other)
	{
		*this = Other;
	}
	FRHITextureDesc(
		ETextureDimension InDimension,
		EPixelFormat InFormat,
		FIntPoint  InExtent,
		uint16 InDepth,
		uint16 InArraySize,
		uint8 InNumMips,
		uint8 InNumSamples,
		uint32 InExtData
	) :
		Dimension(InDimension),
		Format(InFormat),
		Extent(InExtent),
		Depth(InDepth),
		ArraySize(InArraySize),
		NumMips(InNumMips),
		NumSamples(InNumSamples),
		ExtData(InExtData)
	{}
	FRHITextureDesc(ETextureDimension InDimension) :
		Dimension(InDimension)
	{}


	bool IsTextureCube() const
	{
		return Dimension == ETextureDimension::TextureCube || Dimension == ETextureDimension::TextureCubeArray;
	}
	bool IsTexture2D() const
	{
		return Dimension == ETextureDimension::Texture2D || Dimension == ETextureDimension::Texture2DArray;
	}
	bool IsTexture3D() const
	{
		return Dimension == ETextureDimension::Texture3D;
	}
	bool IsTextureArray() const
	{
		return Dimension == ETextureDimension::Texture2DArray || Dimension == ETextureDimension::TextureCubeArray;
	}

	uint32 ExtData = 0;
	FIntPoint Extent = FIntPoint(1, 1);
	uint16 ArraySize = 1;
	uint8 NumMips = 1;
	uint8 Depth = 1;
	uint8 NumSamples = 1;
	ETextureDimension Dimension = ETextureDimension::Texture2D;
	EPixelFormat Format = PF_Unknown;
	ERHIAcces InitialState = ERHIAcces::Unknown;
	ETextureCreateFlags Flags = ETextureCreateFlags::None;
};


struct FRHITextureCreateDesc : public FRHITextureDesc
{
	static FRHITextureCreateDesc Create(const TCHAR* InDebugName, ETextureDimension InDimension)
	{
		return FRHITextureCreateDesc(InDebugName, InDimension);
	}
	static FRHITextureCreateDesc Create2D(const TCHAR* InDebugName)
	{
		return FRHITextureCreateDesc(InDebugName, ETextureDimension::Texture2D);
	}

	FRHITextureCreateDesc& SetExtent(const FIntPoint& InExtent)
	{
		Extent = InExtent;
		return *this;
	}
	FRHITextureCreateDesc& SetFormat(EPixelFormat InFormat)
	{
		Format = InFormat;
		return *this;
	}
	FRHITextureCreateDesc& SetFlags(ETextureCreateFlags InFlags)
	{
		Flags = InFlags;
		return *this;
	}
	FRHITextureCreateDesc& SetInitialState(ERHIAcces InInitialState)
	{
		InitialState = InInitialState;
		return *this;
	}

	FRHITextureCreateDesc() = default;
	FRHITextureCreateDesc(const TCHAR* InDebugName, ETextureDimension InDeminsion) :
		FRHITextureDesc(InDeminsion),
		DebugName(InDebugName)
	{}
	FRHITextureCreateDesc(const FRHITextureDesc& InDesc, const TCHAR* InDebugName) :
		FRHITextureDesc(InDesc),
		DebugName(InDebugName)
	{}




	const TCHAR* DebugName = nullptr;




};




class FRHITexture : public FRHIResource 
{
public:
	EPixelFormat GetPixelFormat() const { return TextureDesc.Format; }

	virtual void* GetNativeResource() const { return nullptr; }
	virtual void* GetNativeShaderResourceView() const { return nullptr; }
	virtual FIntPoint GetSize() const { return TextureDesc.Extent; }
	virtual const FRHITextureDesc& GetDesc() const { return TextureDesc; }

protected:
	FRHITexture(const FRHITextureCreateDesc& InDesc) :
		FRHIResource(RRT_Texture),
		TextureDesc(InDesc)
	{
		SetName(InDesc.DebugName);
	}




protected:
	FRHITextureDesc TextureDesc;

};





class FRHIViewport : public FRHIResource
{
public:
	FRHIViewport() : FRHIResource(RRT_Viewport)  {}

	virtual void* GetNativeSwapChain() const { return nullptr; }

	virtual void Tick(float DeltaTime) {}
	virtual void WaitForFrameEventCompletion() {}
	virtual void Resize(int32 Width, int32 Height, bool bWasMinimized) {}

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


	uint64 ShaderHash = 0;

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


struct FVertexElement
{
	uint8 Offset;
	uint8 Stride;
	uint8 AttributeIndex;
	uint8 bUseInstanceIndex;
	EVertexElementType Type;

	FVertexElement() {}

	FVertexElement(EVertexElementType InType, uint8 InAttributeIndex, uint8 InOffset, uint8 InbUseInstanceIndex, uint8 InStride) :
		Type(InType),
		AttributeIndex(InAttributeIndex),
		Offset(InOffset),
		bUseInstanceIndex(InbUseInstanceIndex),
		Stride(InStride)
	{}
};

template<>
struct std::hash<FVertexElement>
{
	uint64 operator()(const FVertexElement& Other) const
	{
		uint64 Result = 0;
		hash_combine(Result, Other.Offset);
		hash_combine(Result, Other.Stride);
		hash_combine(Result, Other.AttributeIndex);
		hash_combine(Result, Other.bUseInstanceIndex);
		hash_combine(Result, (uint8)Other.Type);
		return Result;
	}
};


typedef TArray<FVertexElement> FVertexDeclarationElementList;

class FRHIVertexDeclaration : public FRHIResource
{
public:
	FRHIVertexDeclaration() : FRHIResource(RRT_VertexDeclaration) {}
	virtual bool GetInitializer(FVertexDeclarationElementList& Init) { return false; }
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

class FRHIGeometryShader : public FRHIGraphicsShader
{
public:
	FRHIGeometryShader() : FRHIGraphicsShader(RRT_GeometryShader, ST_Geometry)
	{}
};


class FRHIComputeShader : public FRHIShader
{
public:
	FRHIComputeShader() : FRHIShader(RRT_ComputeShader, ST_Geometry)
	{}
};

class FRHISamplerState : public FRHIResource
{
	FRHISamplerState() : FRHIResource(RRT_SamplerState) {}
	virtual FRHIDescriptorHandle GetBindlessHandle() const { return FRHIDescriptorHandle(); }
};

class FRHIRenderTargetView
{
public:
	FRHIRenderTargetView() = default;


	FRHITexture* Texture = nullptr;
	uint32 MipIndex = 1;
	uint32 ArraySlice = 0;
private:


};




class FRHISetRenderTargetInfo;

class FRHIDepthRenderTargetView
{
public:
	FRHITexture* Texture;


private:
};

struct FRHIRenderPassInfo
{
	struct FColorEntry
	{
		FRHITexture* RenderTarget = nullptr;
		int32 ArraySlice = -1;
		uint8 MipIndex = 0;
	};
	TStaticArray<FColorEntry, 8> ColorRenderTargets;

	struct FDepthStencilEntry
	{
		FRHITexture* DepthStencilTarget = nullptr;
	};

	FDepthStencilEntry DepthStencilEntry;

	FRHIRenderPassInfo() = default;
	FRHIRenderPassInfo(FRHITexture* ColorRT)
	{
		check(ColorRT);
		ColorRenderTargets[0].RenderTarget = ColorRT;
	}
	FRHIRenderPassInfo(uint32 NumColorRT, FRHITexture** ColorRT)
	{
		check(NumColorRT > 0);
		for (int32 i = 0; i < NumColorRT; ++i)
		{
			check(ColorRT[i]);
			ColorRenderTargets[i].RenderTarget = ColorRT[i];
		}
	}

	FRHISetRenderTargetInfo ConvertToRenderTargetInfo() const;


};


class FRHISetRenderTargetInfo
{
public:
	FRHIRenderTargetView ColorRenderTarget[MAX_RENDER_TARGETS];
	int32 NumColorRenderTargets = 0;
	bool bClearColor = false;

	FRHIDepthRenderTargetView DepthStencilRenderTarget;
	bool bClearDepth = false;
	bool bClearStencil = false;

	//...
};




struct FBoundShaderStateInput
{
	FBoundShaderStateInput
	(
		FRHIVertexShader* InVertexShader,
		FRHIPixelShader* InPixelShader,
		FRHIGeometryShader* InGeometryShader
	):
		VertexShaderRHI(InVertexShader),
		PixelShaderRHI(InPixelShader),
		GeometryShaderRHI(InGeometryShader)
	{}

	FBoundShaderStateInput()
	{}

	bool operator ==(const FBoundShaderStateInput& Other) const
	{
		return VertexShaderRHI == Other.VertexShaderRHI && PixelShaderRHI == Other.PixelShaderRHI && GeometryShaderRHI == Other.GeometryShaderRHI && VertexDeclaration == Other.VertexDeclaration; 
	}

	FRHIVertexDeclaration* VertexDeclaration = nullptr;
	FRHIVertexShader* VertexShaderRHI = nullptr;
	FRHIPixelShader* PixelShaderRHI = nullptr;
	FRHIGeometryShader* GeometryShaderRHI = nullptr;
};



class FGraphicsPipelineStateInitializer
{
public:
	FBoundShaderStateInput BoundShaderState;
	EPrimitiveType PrimitiveType;


	bool operator==(const FGraphicsPipelineStateInitializer& Other) const
	{
		return Other.BoundShaderState == BoundShaderState && PrimitiveType == Other.PrimitiveType;
	}

private:
};

template<>
struct std::hash<FBoundShaderStateInput>
{
	std::size_t operator()(const FBoundShaderStateInput& Key) const
	{
		std::size_t Result = 0;
		hash_without_hash_combine(Result, Key.PixelShaderRHI ? Key.PixelShaderRHI->ShaderHash : 0);
		hash_without_hash_combine(Result, Key.VertexShaderRHI ? Key.VertexShaderRHI->ShaderHash : 0);
		hash_without_hash_combine(Result, Key.GeometryShaderRHI ? Key.GeometryShaderRHI->ShaderHash : 0);
		//hash_without_hash_combine(Result, Key.PixelShaderRHI ? Key.VertexDeclaration->ShaderHash : 0);

		return Result;
		/*return(std::hash<uint64>()((uint64)Key.VertexShaderRHI) << 1)
			^ (std::hash<uint64>()((uint64)Key.PixelShaderRHI) << 1)
			^ (std::hash<uint64>()((uint64)Key.GeometryShaderRHI) << 1);*/
	}

};


template<>
struct std::hash<FGraphicsPipelineStateInitializer>
{
	std::size_t operator()(const FGraphicsPipelineStateInitializer& Key) const
	{	 
		std::size_t Result = 0;
		hash_combine(Result, Key.BoundShaderState);
		hash_combine(Result, (uint32)Key.PrimitiveType);
		return Result;
	}

};