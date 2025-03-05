// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "BBase.generated.h"

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBBase: public UPrimaryDataAsset {
	GENERATED_BODY()

public:
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

	UFUNCTION(BlueprintCallable)
	float TopWant(FName& OToken);

	UFUNCTION(BlueprintCallable)
	float TopNeed(FName& OToken);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> Tokens;
};
