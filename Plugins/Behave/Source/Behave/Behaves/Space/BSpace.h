// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"

#include "BSpace.generated.h"

// TODo since this is a proto. this class encompasses ALL aspects
// later on, i could split each aspect into its own thing

// by space I mean "physical" mostly. but that's ambiguous. (moving, doing certain actions, maybe just moving maybe other behavs can do physical actions too, dunno)

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBSpace: public UBBase {
	GENERATED_BODY()

public:
	UBSpace();

	static inline FName T_Move = "Space.Move";
	static inline FName T_Eat = "Space.Eat";
	static inline FName T_Sleep = "Space.Sleep";

protected:
	virtual void Tick_Implementation(const float DT) override;
	virtual void ReactState_Implementation(const float DT, const FName& Token, const float Val = 0) override;
	virtual float Want_Implementation(const FName& Token) override;
	virtual EBDoRes Do_Implementation(const float DT, FName& IOToken) override;

	float MoveTime = 0; // simulate moving to a place by adding a delay. at this stage the actual position and stuff matters not.
	bool Moved = false;
	float EatTime = 0; // simulate eating with a delay
	float SleepTime = 0; // simulate with a delay
};
