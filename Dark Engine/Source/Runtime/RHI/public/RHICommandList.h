#pragma once
#include "Templates/DarkTemplate.h"
#include "CoreTypes.h"
#include "RHIResources.h"

class FGraphEventRef;

class FRHICommandListBase : public FNoncopyble
{
public:
	enum class ERecordingThread
	{
		Render,
		Any
	};

protected:
	FRHICommandListBase(ERecordingThread InRecordingThread) {}

public:
	FRHICommandListBase(FRHICommandListBase&& Other) {}
	~FRHICommandListBase() {}

	bool IsImmediate();
	class FRHICommandListImmediate& GetAsImmediate();
	const int32 GetUsedMemory() const;


	void AddDispatchPrerequisite(const FGraphEventRef& InPrereq);



	void FinishRecording();

	void* Alloc(int64 InAlocSize, int64 InAllignment);


};

class FRHIComputeCommandList : public FRHICommandListBase
{
public:
	FRHIComputeCommandList(ERecordingThread InRecordingThread = ERecordingThread::Render)
		: FRHICommandListBase(InRecordingThread)
	{

	}

protected:

};


class FRHICommandList : public FRHIComputeCommandList
{
public:
	FRHICommandList(ERecordingThread InRecordingThread = ERecordingThread::Render)
		:	FRHIComputeCommandList(InRecordingThread)
	{

	}

};





class FRHICommandListImmediate : public FRHICommandList
{
	friend class FRHICommandListExecutor;

	FRHICommandListImmediate()
		: FRHICommandList(ERecordingThread::Render)
	{

	}
	

	void ExecuteAndReset();
	void Reset();

public:
	void BeginScene();
	void EndScene();
	void BeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTargetRHI) {}
	void EndDrawingViewport(FRHIViewport* Viewport, bool bPresent, bool bVsync);
	void BeginFrame();
	void EndFrame();



};




class FRHICommandListExecutor
{
public:
	FRHICommandListExecutor()
		: bLatchedByPass(false)
	{

	}

	static FRHICommandListImmediate& GetImmediateCommandList();
	void LatchByPass() {}




private:
	FRHICommandListImmediate CommandListImmediate;
	bool bLatchedByPass;


};

extern FRHICommandListExecutor GRHICommandList;