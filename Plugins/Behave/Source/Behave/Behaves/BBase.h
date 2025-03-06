// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BConsts.h"

#include "BBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBBaseTrait, const FBTrait&, Trait, const bool, Add);

// base for behaviors. the idea is that you make your own and each one handles what it needs
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBBase: public UObject {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Begin();

	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void End();

	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Tick(const float DT);

	// returns the value it has for a token. if it's boolean then it's 0: false, 1:true
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	float Val(const FName& Token) const;

	// returns what it wants the most. with a normalized value of how much it wants it.
	// try return -1 if nothing to want.
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	float Want(const FName& Token);

	// attempt to do something.
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	EBDoRes Do(const float DT, FName& IOToken);

	// react to a state
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void ReactState(const float DT, const FName& Token, const float Val=0);

	// react to action being acted on. returns true on need to replan.
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	bool ReactDo(const float DT, const FName& Token);

	UFUNCTION(BlueprintCallable)
	float TopWant(FName& OToken);

	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Dump();

	// will clamp
	UFUNCTION(BlueprintCallable, meta=(ForceAsFunction))
	void Mod(const FName& Token, const float Dif);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, float> Values;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FBWantNorm> WantNorms;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WantMin = .5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	FBBaseTrait OnTrait;
};
