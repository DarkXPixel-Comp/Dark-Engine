#pragma once
#include "Delegate/Delegate.h"


class FCoreDelegates
{
public:
	static TMultiCastDelegate<void> OnInit;
	static TMultiCastDelegate<void> OnPreInit;
	static TMultiCastDelegate<void> OnPostInit;
	static TMultiCastDelegate<void> OnTick;
	static TMultiCastDelegate<void> OnPreMain;
	static TMultiCastDelegate<void>	OnExitRequest;


};