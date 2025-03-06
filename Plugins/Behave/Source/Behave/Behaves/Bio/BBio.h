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
	UBBio();
	
	static const inline FName T_Tired = "Bio.Tired";
	static const inline FName T_Cold = "Bio.Cold";
	static const inline FName T_Hungry = "Bio.Hungry";

protected:
	virtual void Tick_Implementation(const float DT) override;
	virtual void ReactState_Implementation(const float DT, const FName& Token, const float Val = 0) override;
	virtual EBDoRes ReactDo_Implementation(const float DT, FName& IOToken) override;

	static inline float BioDampE = .01;
	static inline float BioDampF = .03;
	// static inline float BioDampW = .001;
};
