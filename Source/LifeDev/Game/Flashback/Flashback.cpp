// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#include "Flashback.h"

UFlashback::UFlashback() {}

void UFlashback::SetVal(float New) {
	New = FMath::Clamp(New, 0.0f, 1.0f);
	if (FMath::IsNearlyEqual(New, Val)) return;
	Val = New;
	OnChange.Broadcast(Val);
}

void UFlashback::IncVal(float By) {
	SetVal(Val+By);
}
