// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"
#include "BEmo.generated.h"

// TODo since this is a proto. this class encompasses ALL emotions

// later on, i could split each emotion into its own thing

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBEmo: public UBBase {
	GENERATED_BODY()

public:
	static const inline FName T_Anger = "Emo.Anger";
	static const inline FName T_Sad = "Emo.Sad";
	static const inline FName T_Fear = "Emo.Fear";
	static const inline FName T_Shame = "Emo.Shame"; // not same as guilt, but we can fake one with the other
	static const inline FName T_Joy = "Emo.Joy";
	static const inline FName T_Bore = "Emo.Bore";

	TArray<FName> Tokens = {
		T_Anger, T_Sad, T_Fear, T_Shame, T_Joy, T_Bore
	};

	TMap<FName, float> Values = {
		{T_Anger, 0},
		{T_Sad, 0},
		{T_Fear, 0},
		{T_Shame, 0},
		{T_Bore, 0}
	};
protected:
	virtual void Tick_Implementation(const float DT) override;
	
	static inline float EmoDamp = .001;
};
