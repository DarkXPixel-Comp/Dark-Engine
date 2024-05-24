#include "ObjectBase.h"
#include "Containers/UnordoredMap.h"

struct FPendingRegistrant
{
	GObjectBase* Object = nullptr;
	FPendingRegistrant* NextAutoRegister = nullptr;

	FPendingRegistrant(GObjectBase* InObject):
		Object(InObject)
	{}
};

static FPendingRegistrant* GFirstPendingRegistrant = nullptr;
static FPendingRegistrant* GLastPendingRegistrant = nullptr;




struct FPendingRegistrantInfo
{
	const TCHAR* Name;
	FPendingRegistrantInfo(const TCHAR* InName) :
		Name(InName)
	{

	}

	static TMap<GObjectBase*, FPendingRegistrantInfo>& GetMap()
	{
		static TMap<GObjectBase*, FPendingRegistrantInfo> RegistrantInfo;
		return RegistrantInfo;
	}
};


void GObjectBase::Register(const TCHAR* InName)
{
	TMap<GObjectBase*, FPendingRegistrantInfo>& MapRegistrants = FPendingRegistrantInfo::GetMap();

	FPendingRegistrant* PendingRegistration = new FPendingRegistrant(this);

	MapRegistrants.emplace(this, FPendingRegistrantInfo(InName));

	if (GLastPendingRegistrant)
	{
		GLastPendingRegistrant->NextAutoRegister = PendingRegistration;
	}
	else
	{
		GFirstPendingRegistrant = PendingRegistration;
	}

	GLastPendingRegistrant = PendingRegistration;

}