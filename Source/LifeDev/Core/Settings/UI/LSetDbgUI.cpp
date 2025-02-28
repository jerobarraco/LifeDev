// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetDbgUI.h"

#include "LifeDev/Core/Settings/LSettings.h"

void ULSetDbgUI::Load_Implementation() {
	Super::Load_Implementation();
	const bool Dbg = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
	if (LIKELY(!Dbg)) return;
}
