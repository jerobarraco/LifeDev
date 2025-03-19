// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMoveEat.h"

static const FVector FoodPos(50, 50, 50);

void UBMoveEat::SetTarget_Implementation() {
	// i know this is lame. this is stub. in a real case you'd want to find the food pos.
	Target = FoodPos;
}
