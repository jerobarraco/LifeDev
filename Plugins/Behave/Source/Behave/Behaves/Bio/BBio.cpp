// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBio.h"

void UBBio::Tick_Implementation(const float DT) {
	Super::Tick_Implementation(DT);

	Values[T_Energy] -= BioDampE*DT;
	Values[T_Full] -= BioDampF*DT;
}
