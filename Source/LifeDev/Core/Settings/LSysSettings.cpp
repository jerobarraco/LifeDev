#include "LSysSettings.h"

#include "JUtils/JMiscUtils.h"

ULSysSettings* ULSysSettings::Get() {
	return Cast<ULSysSettings>(StaticClass()->GetDefaultObject());
}

FName ULSysSettings::GetCategoryName() const {
	static const FName Cat = FName("LifeDev"); 
	return Cat;
}

bool ULSysSettings::ShouldUseDebugFeats() {
	return UJMiscUtils::IsDebug() ? UseDebugFeats : false;
}

bool ULSysSettings::ShouldUseSaveGame() {
	return UJMiscUtils::IsDebug() ? UseSaveGame : true;
}

TSet<EFeat>& ULSysSettings::GetFeats() {
	const bool UseDebug = UseDebugFeats && UJMiscUtils::IsDebug();
	return UseDebug ? DebugFeats : DefaultFeats;
}
