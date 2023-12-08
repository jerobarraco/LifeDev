// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LifeDev/Core/Settings/LSave.h"

#include "LSysSettings.h"


void ULSave::Reset() {
	ChapterID = ULSysSettings::Get()->StartChap;
}
