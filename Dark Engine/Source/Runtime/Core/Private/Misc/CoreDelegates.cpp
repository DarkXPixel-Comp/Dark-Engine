#include "Misc/CoreDelegates.h"

TMultiCastDelegate<void> FCoreDelegates::OnInit;
TMultiCastDelegate<void> FCoreDelegates::OnPreInit;
TMultiCastDelegate<void> FCoreDelegates::OnPostInit;
TMultiCastDelegate<void> FCoreDelegates::OnTick;
TMultiCastDelegate<void> FCoreDelegates::OnPreMain;




/*	static TMultiCastDelegate<void> OnInit;
	static TMultiCastDelegate<void> OnPreInit;
	static TMultiCastDelegate<void> OnPostInit;
	static TMultiCastDelegate<void> OnTick;
	static TMultiCastDelegate<void> OnPreMain;*/