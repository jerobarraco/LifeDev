// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Behave/Behaves/BBase.h"

#include "BSpace.generated.h"

// TODo since this is a proto. this class encompasses ALL aspects
// later on, i could split each aspect into its own thing

// by space I mean "physical" mostly. but that's ambiguous. (moving, doing certain actions, maybe just moving maybe other behavs can do physical actions too, dunno)

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FBSpaceFoodClose);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FBSpaceMoveTo, const FVector&, Point);
DECLARE_DYNAMIC_DELEGATE(FBSpaceMoveToFood);
DECLARE_DYNAMIC_DELEGATE(FBSpaceMoveToSleep);
DECLARE_DYNAMIC_DELEGATE(FBSpaceMoveToPlay);

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBSpace: public UBBase {
	GENERATED_BODY()

public:
	UBSpace();

	static inline FName T_Move = "Space.Move";
	static inline FName T_Eat = "Space.Eat";
	static inline FName T_Sleep = "Space.Sleep";
	static inline FName T_Play = "Space.Play";

	UFUNCTION(CallInEditor)
	void MoveStop();
	
	// called when trying to eat. return true if it can.
	UPROPERTY(BlueprintReadWrite, Transient)
	FBSpaceFoodClose OnFoodClose;

	UPROPERTY(BlueprintReadWrite, Transient)
	FBSpaceMoveToFood OnMoveToFood;

	UPROPERTY(BlueprintReadWrite, Transient)
	FBSpaceMoveToSleep OnMoveToSleep;
	UPROPERTY(BlueprintReadWrite, Transient)
	FBSpaceMoveToPlay OnMoveToPlay;

protected:
	virtual void End_Implementation() override;
	virtual void Tick_Implementation(const float DT) override;
	virtual void ReactState_Implementation(const float DT, const FName& Token, const float Val = 0) override;
	virtual float Want_Implementation(const FName& Token) override;
	virtual EBDoRes Do_Implementation(const float DT, FName& IOToken) override;
	
	float MoveTime = 0; // simulate moving to a place by adding a delay. at this stage the actual position and stuff matters not.
	bool Moved = false;
	bool MovedSleep = false;
	bool MovedPlay = false;
	float EatTime = 0; // simulate eating with a delay
	float SleepTime = 0; // simulate with a delay
	float PlayTime = 0; // simulate with a delay
};
