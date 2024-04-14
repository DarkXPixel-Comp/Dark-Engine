#include "D3DShaderFormat.h"
#include "dxcapi.h"
#include "Templates/RefCounting.h"
#include "ShaderCore.h"
#include <RHIDefines.h>
#include "Misc/Paths.h"


const TCHAR* GetShaderProfileName(uint32 ShaderType)
{
	switch (ShaderType)
	{
	case ST_Vertex:
		return TEXT("vs_6_6");
	case ST_Mesh:
		return TEXT("ms_6_6");
	case ST_Pixel:
		return TEXT("ps_6_6");
	case ST_Geometry:
		return TEXT("gs_6_6");
	case ST_Compute:
		return TEXT("cs_6_6");
	}

}


struct FDxcArguments
{
public:
	FDxcArguments
	(
		const FShaderCompilerInput& Input,
		const FString& InEntryPoint,
		const FString& InShaderProfile,
		const FString& InSourcePath
	):
		EntryPoint(InEntryPoint),
		Filename(Input.SourceFilePath),
		ShaderProfile(InShaderProfile),
		SourcePath(InSourcePath)
	{

	}

	void GetCompilerArgs(TArray<const WCHAR*>& Out) const
	{
		Out.Add(TEXT("-E"));
		Out.Add(*EntryPoint);
		Out.Add(TEXT("-T"));
		Out.Add(*ShaderProfile);
		Out.Add(*SourcePath);
	}



private:
	FString EntryPoint;
	FString Filename;
	FString SourcePath;
	FString ShaderProfile;
	TArray<FString>	ExtraArguments;
};



bool PreprocessD3DShader(const FShaderCompilerInput& Input, const FShaderCompilerEnvironment& MergedEvironment, FShaderPreprocessOutput& PreprocessOutput)
{
	FShaderCompilerDefinitions Defines;

	return true;

}

void CompileD3DShader(const FShaderCompilerInput& Input, const FShaderPreprocessOutput&, FShaderCompilerOutput& Output, const FString& WorkingDirectory)
{
	TRefCountPtr<IDxcCompiler3>	Compiler;
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&Compiler));

	TRefCountPtr<IDxcLibrary> Library;
	DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&Library));

	TRefCountPtr<IDxcUtils>	Utils;
	DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&Utils));

	FString SourcePath = FString::PrintF(TEXT("%s%s"), *FPaths::EngineShaderDir(), *Input.SourceFilePath);

	TRefCountPtr<IDxcBlobEncoding> TextBlob;
	Library->CreateBlobFromFile(*SourcePath, nullptr, &TextBlob);

	FDxcArguments Args(Input, Input.EntryPoint, GetShaderProfileName(Input.ShaderType), SourcePath);
	TArray<const WCHAR*> OutArgs;
	Args.GetCompilerArgs(OutArgs);
	

	if (!TextBlob)
	{
		return;
	}
	DxcBuffer SourceBuffer;
	SourceBuffer.Encoding = DXC_CP_ACP;
	SourceBuffer.Ptr = TextBlob->GetBufferPointer();
	SourceBuffer.Size = TextBlob->GetBufferSize();

	

	TRefCountPtr<IDxcIncludeHandler> IncludeHandler;
	Utils->CreateDefaultIncludeHandler(&IncludeHandler);


	TRefCountPtr<IDxcResult> CompileResult;
	Compiler->Compile(&SourceBuffer, OutArgs.GetData(), OutArgs.Num(),
		IncludeHandler.Get(), IID_PPV_ARGS(&CompileResult));

	HRESULT Result;
	CompileResult->GetStatus(&Result);

	TRefCountPtr<IDxcBlob> OutResult;
	

	if (SUCCEEDED(Result))
	{
		TRefCountPtr<IDxcBlobUtf16>	ObjectCodeNameBlob;
		check(CompileResult->HasOutput(DXC_OUT_OBJECT) && "No object code found");
		CompileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&OutResult), &ObjectCodeNameBlob);		
	}
	else
	{
		TRefCountPtr<IDxcBlobEncoding> ErrorBuffer;
		CompileResult->GetErrorBuffer(&ErrorBuffer);
		std::string Error = (char*)ErrorBuffer->GetBufferPointer();
		DE_LOG(LogShaders, Error, TEXT("%s"), *FString(Error));
		return;
	}
	
	Output.ShaderCode.ShaderCodeWithOptionalData.Resize(OutResult->GetBufferSize());
	FMemory::Memcpy(Output.ShaderCode.ShaderCodeWithOptionalData.GetData(),
		OutResult->GetBufferPointer(), OutResult->GetBufferSize());
	Output.ShaderHash = std::hash<TArray<uint8>>{}(Output.ShaderCode.ShaderCodeWithOptionalData);
}

