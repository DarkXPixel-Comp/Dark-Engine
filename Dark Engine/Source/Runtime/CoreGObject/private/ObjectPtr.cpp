#include "ObjectPtr.h"
#include "Object.h"


GClass* FObjectPtr::GetClass() const
{
	return Get()->GetClass();
}
