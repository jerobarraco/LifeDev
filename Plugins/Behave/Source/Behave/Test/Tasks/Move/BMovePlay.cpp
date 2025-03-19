// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMovePlay.h"

static const FVector PlayPos(10, -50, 30);

void UBMovePlay::SetTarget_Implementation() {
	Target = PlayPos;
}
