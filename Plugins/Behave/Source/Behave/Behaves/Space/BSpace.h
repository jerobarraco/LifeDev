// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"

#include "BSpace.generated.h"

// TODo since this is a proto. this class encompasses ALL aspects
// later on, i could split each aspect into its own thing

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBSpace: public UBBase {
	GENERATED_BODY()

public:
	UBSpace();
	

protected:
	virtual void Tick_Implementation(const float DT) override;
	virtual void React_Implementation(const float DT, const FName& Token, const float Val = 0) override;
	virtual float Want_Implementation(const FName& Token) override;
	virtual EBDoRes Do_Implementation(const FName& Token) override;

	// simulate moving to a place by adding a delay
	float MoveTime = 0;
};
