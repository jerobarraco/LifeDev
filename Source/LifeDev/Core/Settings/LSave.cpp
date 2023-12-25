// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LifeDev/Core/Settings/LSave.h"

#include "LSysSettings.h"

void ULSave::Reset() {
	// ChapterID = ULSysSettings::IsDebugBuild() ? ULSysSettings::Get()->StartChap : 0;
	// i want to preserve the ability to skip chapters even on shipping builds
	ChapterID = ULSysSettings::Get()->StartChap;
}
