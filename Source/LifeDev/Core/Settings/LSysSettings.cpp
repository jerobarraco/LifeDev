
#include "LSysSettings.h"

ULSysSettings* ULSysSettings::Get()
{
	return Cast<ULSysSettings>(StaticClass()->GetDefaultObject());
}

FName ULSysSettings::GetCategoryName() const {
	static const FName Cat = FName("LifeDev"); 
	return Cat;
}
