// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT


#pragma once

#include "BBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBOnGives, const FName&, Token, const float, Val);

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
	float Has(const FName& Token);

	// returns what it wants the most. with a normalized value of how much it wants it.
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	float Want(const FName& Token);

	// returns what it needs the most. with a normalized value of how much it wants it.
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	float Need(const FName& Token);

	// attempt to do something.
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Do(const FName& Token);

	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Affect(const FName& Token, const float Val=0);

	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void React(const FName& Token, const float Val=0);

	UFUNCTION(BlueprintCallable)
	float TopWant(FName& OToken);

	UFUNCTION(BlueprintCallable)
	float TopNeed(FName& OToken);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> Tokens;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, float> Values;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FBOnGives OnGives;
	// TODO at some point tokens and values can be merged
};

// TODO æfectors
