// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"
#include "BEmo.generated.h"

// TODo since this is a proto. this class encompases ALL emotions

// later on, i could split each emotion into its own thing

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBEmo: public UBBase {
	GENERATED_BODY()

public:
	static const inline FName T_Anger = "Emo.Anger";
	static const inline FName T_Sad = "Emo.Sad";
	static const inline FName T_Fear = "Emo.Fear";
	static const inline FName T_Shame = "Emo.Shame";
	static const inline FName T_Bore = "Emo.Bore";

	TArray<FName> Tokens = {
		T_Anger, T_Sad, T_Fear, T_Shame, T_Bore
	};

protected:
};
