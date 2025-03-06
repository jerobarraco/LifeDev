// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "Behaves/BConsts.h"
#include "Components/ActorComponent.h"

#include "CBehave.generated.h"

struct FBTrait;
enum class EBDoRes: uint8;
class UBBase;

// TODO improve the doing so it only triggers once.. like predo.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCBehaveDoPre, const FName&, Token); // for lack of better word. WIP
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCBehaveDo, const FName&, Token, const float, DT);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Behave), meta=(BlueprintSpawnableComponent))
class BEHAVE_API UCBehave: public UActorComponent {
	GENERATED_BODY()
public:
	UCBehave();

	UFUNCTION(BlueprintCallable)
	void ReactState(const float DT, const FName& Name, const float V=0);

	UFUNCTION(BlueprintCallable)
	void TraitMod(const FBTrait& Trait, const bool Add);
	
	UFUNCTION(BlueprintCallable)
	void TraitAdd(const FBTrait& Trait);
	UFUNCTION(BlueprintCallable)
	void TraitRem(const FName& Name);
	UFUNCTION(BlueprintCallable)
	FBTrait TraitGet(const FName& Name);
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	UBBase* GetBehave(const TSubclassOf<UBBase> Class);

	// experimental/stub
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FBTrait> Traits;

	UPROPERTY(BlueprintAssignable, Transient)
	FCBehaveDo OnDo;
	
	UPROPERTY(BlueprintAssignable, Transient)
	FCBehaveDoPre OnDoPre;
	
protected:
	virtual void TickComponent(const float DeltaTime, const enum ELevelTick TickType,
		FActorComponentTickFunction* const ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void Dump();
	UFUNCTION()
	void WhatWant();
	UFUNCTION()
	void Do(const float DT);
	float TraitWantMod(const FName& Token, float V);
	void PlanCheck();
	void RePlan();
	void ReactAllStates(float DeltaTime);

	// leave the object null pls
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UBBase>, TObjectPtr<UBBase>> Behaves;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EBDoRes DoRes;

	// a plan is always for ONE want, a very very simple plan
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FName> Plan;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float PlanVal = 0; // TODO is this even useful? at least for debug on outliner. there's a case when to use it, what was it?

};
