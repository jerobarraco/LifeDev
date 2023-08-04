// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#include "Flashback.h"
UFlashback::UFlashback() {}

void UFlashback::SetVal(float New) {
	if (FMath::IsNearlyEqual(New, Val)) return;
	Val = New;
	OnChange.Broadcast(Val);
}
