
#include "LSysSettings.h"

ULSysSettings* ULSysSettings::Get()
{
	return Cast<ULSysSettings>(StaticClass()->GetDefaultObject());
}

FName ULSysSettings::GetCategoryName() const {
	static const FName Cat = FName("LifeDev"); 
	return Cat;
}

bool ULSysSettings::IsDebugBuild() {
#if (UE_BUILD_TEST || UE_BUILD_SHIPPING)
	return false;
#else
	return true;
#endif
}
