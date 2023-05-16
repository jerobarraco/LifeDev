
#include "LSysSettings.h"

ULSysSettings* ULSysSettings::Get()
{
	return Cast<ULSysSettings>(StaticClass()->GetDefaultObject());
}
