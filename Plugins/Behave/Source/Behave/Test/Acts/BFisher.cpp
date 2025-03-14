// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BFisher.h"

#include "Behave/CBehave.h"

#include "Behave/Test/BFish.h"


void UBFisher::Register_Implementation(UCBehave* const B) {
	Super::Register_Implementation(B);
	if (UNLIKELY(!B)) return;

	Fish = Cast<ABFish>(B->GetOwner());
}
