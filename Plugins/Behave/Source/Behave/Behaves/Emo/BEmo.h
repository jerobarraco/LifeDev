// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"
#include "BEmo.generated.h"

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBEmo: public UBBase {
	GENERATED_BODY()

public:
	static const inline FName T_Anger = "Emo.Anger";
	static const inline FName T_Sad = "Emo.Sad";
	static const inline FName T_Fear = "Emo.Fear";
	static const inline FName T_Shame = "Emo.Shame";

	// has (knows)
	// want
	// need
	// do
};
