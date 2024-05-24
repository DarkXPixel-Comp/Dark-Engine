#include "Object.h"


FObjectInitializer* CurrentInitializer = nullptr;




void InitGObject()
{


}



FStaticConstructorObjectParameters::FStaticConstructorObjectParameters(const GClass* InClass):
	Class(InClass)
{

}



FObjectInitializer::FObjectInitializer()
{

}

FObjectInitializer& FObjectInitializer::Get()
{
	return *CurrentInitializer;
}


GObject::GObject()
{
	FObjectInitializer* ObjectInitializer = &FObjectInitializer::Get();

}
