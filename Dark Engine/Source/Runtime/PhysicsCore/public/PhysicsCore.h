#pragma once
#include "foundation/PxFoundation.h"
#include "extensions/PxDefaultAllocator.h"
#include "foundation/PxErrorCallback.h"
#include "PxPhysics.h"
#include "Logging/LogMacros.h"
#include "pvd/PxPvd.h"
#include "PxScene.h"
#include "PxSceneDesc.h"


DECLARE_LOG_CATEGORY(PhysXLog, Log);



class FPhysicsCore : public	physx::PxErrorCallback
{
	friend void InitPhysicsCore();
	friend void ShutdownPhysicsCore();
public:
	FPhysicsCore() = default;
	virtual ~FPhysicsCore() noexcept;


	physx::PxPhysics* GetPhysics() const { return Physics; }
	physx::PxScene* CreateScene(physx::PxSceneDesc& SceneDesc);
	const physx::PxTolerancesScale& GetScaleScene() const { return Physics->getTolerancesScale(); }


protected:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;

private:
	void Init();
	void Destroy();






private:
	physx::PxFoundation* Foundation = nullptr;
	physx::PxDefaultAllocator DefaultAllocator;
	physx::PxPhysics* Physics = nullptr;
	physx::PxPvd* Pvd = nullptr;
	physx::PxCpuDispatcher* CpuDispatcher = nullptr;

private:
	bool bPhysXDebug = true;
	bool bInitExtensions = false;
};



extern FPhysicsCore* GPhysicsCore;

extern void InitPhysicsCore();
extern void ShutdownPhysicsCore();