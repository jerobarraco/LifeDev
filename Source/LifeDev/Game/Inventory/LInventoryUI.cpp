 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryUI.h"
void ULInventoryUI::FadeUsed(const bool In) {
	if (UNLIKELY(!AUsed)) return;

	PlayAnimation(AUsed, 0, 1,
		In ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, 1);
}
