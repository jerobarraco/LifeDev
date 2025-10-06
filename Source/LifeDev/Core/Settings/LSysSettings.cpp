#include "LSysSettings.h"

#include "JUtils/Misc/JUtilsSys.h"

ULSysSettings* ULSysSettings::Get() {
	return GetMutableDefault<ULSysSettings>(); // the same as the previous code.
}

FName ULSysSettings::GetCategoryName() const {
	static const FName Cat = FName("LifeDev"); 
	return Cat;
}

bool ULSysSettings::GetUseDebugFeats() const {
	return UseDebugFeats & UJUtilsSys::IsDebug();
}

int32 ULSysSettings::GetStartChap() const {
	const bool UseDebug = GetUseDebugFeats();
	return UseDebug ? StartChap : 0; // force to 0 on release
}
