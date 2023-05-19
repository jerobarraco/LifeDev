
#include "LSysSettings.h"

ULSysSettings* ULSysSettings::Get()
{
	return Cast<ULSysSettings>(StaticClass()->GetDefaultObject());
}

FName ULSysSettings::GetCategoryName() const {
	return FName(TEXT("LifeDev"));
}
