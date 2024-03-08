#pragma once

#include "D3D12RHI.h"
#include "Misc/EnumClassFlags.h"

enum class ED3D12RootSignatureFlags
{
	None = 0,
	AllowMeshShaders = 1 << 0,
	InputAssembler = 1 << 1,
	BindlessResources = 1 << 2,
	BindlessSamplers = 1 << 3,
};

ENUM_CLASS_FLAGS(ED3D12RootSignatureFlags)


namespace D3D12ShaderUtils
{
	namespace StaticRootSignatureConstants
	{
		D3D12_DESCRIPTOR_RANGE_FLAGS SRVDescriptorRangeFlags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE;
		const D3D12_DESCRIPTOR_RANGE_FLAGS CBVDescriptorRangeFlags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
		const D3D12_DESCRIPTOR_RANGE_FLAGS UAVDescriptorRangeFlags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
		const D3D12_DESCRIPTOR_RANGE_FLAGS SamplerDescriptorRangeFlags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
	}
	enum class ERootSignatureRangeType
	{
		CBV,
		SRV,
		UAV,
		Sampler,
	};

	enum class ERootSignatureVisibility
	{
		Vertex,
		Pixel,
		Geometry,
		Mesh,
		Amplification,
		All
	};

	inline D3D12_DESCRIPTOR_RANGE_TYPE GetD3D12DescriptorRangeType(ERootSignatureRangeType Type)
	{
		switch (Type)
		{
		case ERootSignatureRangeType::SRV:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		case ERootSignatureRangeType::UAV:
			return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		case ERootSignatureRangeType::Sampler:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		default:
			return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		}
	}

	inline D3D12_DESCRIPTOR_RANGE_FLAGS GetD3D12DescriptorRangeFlags(ERootSignatureRangeType Type)
	{
		switch (Type)
		{
		case ERootSignatureRangeType::SRV:
			return D3D12ShaderUtils::StaticRootSignatureConstants::SRVDescriptorRangeFlags;
		case ERootSignatureRangeType::CBV:
			return D3D12ShaderUtils::StaticRootSignatureConstants::CBVDescriptorRangeFlags;
		case ERootSignatureRangeType::UAV:
			return D3D12ShaderUtils::StaticRootSignatureConstants::UAVDescriptorRangeFlags;
		case ERootSignatureRangeType::Sampler:
			return D3D12ShaderUtils::StaticRootSignatureConstants::SamplerDescriptorRangeFlags;
		default:
			return D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
		}
	}

	inline D3D12_SHADER_VISIBILITY GetD3D12ShaderVisibility(ERootSignatureVisibility Visibility)
	{
		switch (Visibility)
		{
		case ERootSignatureVisibility::Vertex:
			return D3D12_SHADER_VISIBILITY_VERTEX;
		case ERootSignatureVisibility::Pixel:
			return D3D12_SHADER_VISIBILITY_PIXEL;
		case ERootSignatureVisibility::Geometry:
			return D3D12_SHADER_VISIBILITY_GEOMETRY;
#if !defined(D3D12RHI_TOOLS_MESH_SHADERS_UNSUPPORTED)
		case ERootSignatureVisibility::Mesh:
			return D3D12_SHADER_VISIBILITY_MESH;
		case ERootSignatureVisibility::Amplification:
			return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
#endif
		default:
			return D3D12_SHADER_VISIBILITY_ALL;
		}
	}

	inline const TCHAR* GetVisibilityFlag(ERootSignatureVisibility Visibility)
	{
		switch (Visibility)
		{
		case ERootSignatureVisibility::Vertex:
			return TEXT("SHADER_VISIBILITY_VERTEX");
		case ERootSignatureVisibility::Geometry:
			return TEXT("SHADER_VISIBILITY_GEOMETRY");
		case ERootSignatureVisibility::Pixel:
			return TEXT("SHADER_VISIBILITY_PIXEL");
#if !defined(D3D12RHI_TOOLS_MESH_SHADERS_UNSUPPORTED)
		case ERootSignatureVisibility::Mesh:
			return TEXT("SHADER_VISIBILITY_MESH");
		case ERootSignatureVisibility::Amplification:
			return TEXT("SHADER_VISIBILITY_AMPLIFICATION");
#endif
		default:
			return TEXT("SHADER_VISIBILITY_ALL");
		}
	};


	inline const TCHAR* GetTypePrefix(ERootSignatureRangeType Type)
	{
		switch (Type)
		{
		case ERootSignatureRangeType::SRV:
			return TEXT("SRV(t");
		case ERootSignatureRangeType::UAV:
			return TEXT("UAV(u");
		case ERootSignatureRangeType::Sampler:
			return TEXT("Sampler(s");
		default:
			return TEXT("CBV(b");
		}
	}

	inline const TCHAR* GetFlagName(D3D12_ROOT_SIGNATURE_FLAGS Flag)
	{
		switch (Flag)
		{
		case D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT:
			return TEXT("ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT");
		case D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS:
			return TEXT("DENY_VERTEX_SHADER_ROOT_ACCESS");
		case D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS:
			return TEXT("DENY_GEOMETRY_SHADER_ROOT_ACCESS");
		case D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS:
			return TEXT("DENY_PIXEL_SHADER_ROOT_ACCESS");
		case D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT:
			return TEXT("ALLOW_STREAM_OUTPUT");

#if !defined(D3D12RHI_TOOLS_MESH_SHADERS_UNSUPPORTED)
		case D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS:
			return TEXT("DENY_AMPLIFICATION_SHADER_ROOT_ACCESS");
		case D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS:
			return TEXT("DENY_MESH_SHADER_ROOT_ACCESS");
#endif

		case D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED:
			return TEXT("CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED");
		case D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED:
			return TEXT("SAMPLER_HEAP_DIRECTLY_INDEXED");

		default:
			break;
		}

		return TEXT("");
	};

	struct FRootSignatureCreator
	{
		ED3D12RootSignatureFlags Flags = ED3D12RootSignatureFlags::None;

		virtual ~FRootSignatureCreator() { }

		virtual void AddRootFlag(D3D12_ROOT_SIGNATURE_FLAGS Flag) = 0;
		virtual void AddTable(ERootSignatureVisibility Visibility, ERootSignatureRangeType Type, int32 NumDescriptors, D3D12_DESCRIPTOR_RANGE_FLAGS FlagsOverride = D3D12_DESCRIPTOR_RANGE_FLAG_NONE) = 0;

		void SetFlags(ED3D12RootSignatureFlags InFlags)
		{
			Flags = InFlags;

			if (EnumHasAnyFlags(InFlags, ED3D12RootSignatureFlags::InputAssembler))
			{
				AddRootFlag(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			}

			if (EnumHasAnyFlags(InFlags, ED3D12RootSignatureFlags::BindlessResources))
			{
				AddRootFlag(D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED);
			}

			if (EnumHasAnyFlags(InFlags, ED3D12RootSignatureFlags::BindlessSamplers))
			{
				AddRootFlag(D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED);
			}
		}

		inline bool ShouldSkipType(ERootSignatureRangeType Type)
		{
			if (Type == ERootSignatureRangeType::SRV || Type == ERootSignatureRangeType::UAV)
			{
				return EnumHasAnyFlags(Flags, ED3D12RootSignatureFlags::BindlessResources);
			}

			if (Type == ERootSignatureRangeType::Sampler)
			{
				return EnumHasAnyFlags(Flags, ED3D12RootSignatureFlags::BindlessSamplers);
			}

			return false;
		}
	};



}
