// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

UENUM(Blueprintable, BlueprintType)
enum class EBDoRes : uint8 {
	IGNORE,
	DO,
	NEW,
	FINISH,
	MAX UMETA(HIDDEN)
}; 