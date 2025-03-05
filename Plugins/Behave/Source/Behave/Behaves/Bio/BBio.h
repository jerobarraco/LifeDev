// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"
#include "BBio.generated.h"

// TODo since this is a proto. this class encompasses ALL Bio
// later on, i could split each Bio into its own thing

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBBio: public UBBase {
	GENERATED_BODY()

public:

	static const inline FName T_Energy = "Bio.Energy";
	static const inline FName T_Warm = "Bio.Warm";
	static const inline FName T_Full = "Bio.Full";

	TArray<FName> Tokens = {
		T_Energy, T_Warm, T_Full
	};

	TMap<FName, float> Values = {
		{T_Energy, 1},
		{T_Full, 1},
		{T_Warm, 1},
	};

protected:
	virtual void Tick_Implementation(const float DT) override;
	virtual void React_Implementation(const float DT, const FName& Token, const float Val = 0) override;
	
	static inline float BioDampE = .00001;
	static inline float BioDampF = .00003;
	// static inline float BioDampW = .001;
};
