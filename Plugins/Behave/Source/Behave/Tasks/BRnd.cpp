// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BRnd.h"

#include "JUtils/Misc/JUtilsMisc.h"

bool UBRnd::Plan_Implementation() {
	UJUtilsMisc::ArrayShuffle(Children);
	return Super::Plan_Implementation();
}
