#include "PhysicsCore.h"
#include "foundation/PxPhysicsVersion.h"
#include "common/PxTolerancesScale.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxDefaultCpuDispatcher.h"

using namespace physx;

FPhysicsCore* GPhysicsCore = nullptr;

void InitPhysicsCore()
{
	if (!GPhysicsCore)
	{
		GPhysicsCore = new FPhysicsCore();
		GPhysicsCore->Init();
	}
}

void ShutdownPhysicsCore()
{
	GPhysicsCore->Destroy();
	delete(GPhysicsCore);
}

FPhysicsCore::~FPhysicsCore()
{
	Destroy();
}

PxScene* FPhysicsCore::CreateScene(PxSceneDesc& SceneDesc)
{
	SceneDesc.cpuDispatcher = CpuDispatcher;
	if (SceneDesc.filterShader == PxSimulationFilterShader())
	{
		SceneDesc.filterShader = PxDefaultSimulationFilterShader;
	}
	return Physics->createScene(SceneDesc);
}

void FPhysicsCore::reportError(physx::PxErrorCode::Enum Code, const char* Message, const char* File, int Line)
{
	switch (Code)
	{
	case physx::PxErrorCode::eNO_ERROR:
		DE_LOG(PhysXLog, Log, TEXT("%s"), *FString(Message));
		break;
	case physx::PxErrorCode::eDEBUG_INFO:
		DE_LOG(PhysXLog, Log, TEXT("%s"), *FString(Message));
		break;
	case physx::PxErrorCode::eDEBUG_WARNING:
		DE_LOG(PhysXLog, Warning, TEXT("%s"), *FString(Message));
		break;
	case physx::PxErrorCode::eINVALID_PARAMETER:
		DE_LOG(PhysXLog, Fatal, TEXT("%s"), *FString(Message));
	//	check(false);
		break;
	case physx::PxErrorCode::eINVALID_OPERATION:
		DE_LOG(PhysXLog, Fatal, TEXT("%s"), *FString(Message));
	//	check(false);
		break;
	case physx::PxErrorCode::eOUT_OF_MEMORY:
		DE_LOG(PhysXLog, Fatal, TEXT("%s"), *FString(Message));
		check(false);
		break;
	case physx::PxErrorCode::eINTERNAL_ERROR:
		DE_LOG(PhysXLog, Fatal, TEXT("%s"), *FString(Message));
		check(false);
		break;
	case physx::PxErrorCode::eABORT:
		DE_LOG(PhysXLog, Fatal, TEXT("%s"), *FString(Message));
		check(false);
		break;
	case physx::PxErrorCode::ePERF_WARNING:
		DE_LOG(PhysXLog, Warning, TEXT("%s"), *FString(Message));
		break;
	case physx::PxErrorCode::eMASK_ALL:
		DE_LOG(PhysXLog, Fatal, TEXT("Nu Eto Prosto PISDA %s"), *FString(Message));
		check(false);
		break;
	}
}

void FPhysicsCore::Init()
{
	check(!Foundation);
	check(!Physics);

	Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, DefaultAllocator, *this);
	check(Foundation);
	
	if (bPhysXDebug)
	{
		Pvd = PxCreatePvd(*Foundation);
		check(Pvd);
	}

	Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, PxTolerancesScale(1.f, 1.f), bPhysXDebug, Pvd);
	check(Physics);

	bInitExtensions = PxInitExtensions(*Physics, Pvd);


	const uint32 CountThreads = std::thread::hardware_concurrency();
	CpuDispatcher = PxDefaultCpuDispatcherCreate(CountThreads ? CountThreads / 2 : 1);
}

void FPhysicsCore::Destroy()
{
	if (bInitExtensions)
	{
		PxCloseExtensions();
		bInitExtensions = false;
	}

	if (Physics)
	{
		Physics->release();
		Physics = nullptr;
	}

	if (Pvd)
	{
		Pvd->release();
		Pvd = nullptr;
	}
	if (Foundation)
	{
		Foundation->release();
		Foundation = nullptr;
	}
}
