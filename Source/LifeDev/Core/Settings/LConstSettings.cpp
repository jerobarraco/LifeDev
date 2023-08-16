
#include "LConstSettings.h"

ULConstSettings* ULConstSettings::Get()
{
	return Cast<ULConstSettings>(StaticClass()->GetDefaultObject());
}

FName ULConstSettings::GetCategoryName() const {
	static const FName Cat = FName(TEXT("LifeDev")); 
	return Cat;
}
