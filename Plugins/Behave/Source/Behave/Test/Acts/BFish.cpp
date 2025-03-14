// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BFish.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"


void UBFish::Register_Implementation(UCBehave* const B) {
	Super::Register_Implementation(B);
	if (UNLIKELY(!B)) return;

	Fish = Cast<ABFish>(B->GetOwner());
}
