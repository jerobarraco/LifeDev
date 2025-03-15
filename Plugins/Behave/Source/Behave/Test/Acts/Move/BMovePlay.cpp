// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMovePlay.h"

static const FVector PlayPos(10, -50, 30);
bool UBMovePlay::Plan_Implementation() {
	const bool Res = Super::Plan_Implementation();
	if (!Res) return false;

	// i know this is lame. this is stub. in a real case you'd want to find the food pos.
	Target = PlayPos;

	return true;
}