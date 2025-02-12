#include "LSysSettings.h"

#include "JUtils/Misc/JUtilsMisc.h"

ULSysSettings* ULSysSettings::Get() {
	return GetMutableDefault<ULSysSettings>(); // the same as the previous code.
}

FName ULSysSettings::GetCategoryName() const {
	static const FName Cat = FName("LifeDev"); 
	return Cat;
}

TSet<EFeat>& ULSysSettings::GetFeats() {
	const bool UseDebug = GetUseDebugFeats();
	return UseDebug ? DebugFeats : DefaultFeats;
}

bool ULSysSettings::GetUseDebugFeats() const {
	return UseDebugFeats && UJUtilsMisc::IsDebug();
}
