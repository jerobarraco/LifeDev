// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "StoryTypes.h"

#include "Story.generated.h"

class AStep;
class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStop, const FName& ,Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStart, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStorySeqStop);

// World subsystem to deal with Inventory
UCLASS(Blueprintable, Category="Story")
class STORY_API UStory : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();
	virtual void Init_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeInit();
	virtual void DeInit_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool StartSequence(const TArray<FName>& InSeq);
	virtual bool StartSequence_Implementation(const TArray<FName>& InSeq);

	// starts a step by the name. stops the current one before that.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Start(const FName& Name);
	virtual bool Start_Implementation(const FName& Name);

	// stops a step. if not specified it will stop the current one. then it will start the next.
	// the name is a protection mechanism mostly, used by the story steps.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Stop(const FName& Name=NAME_None);
	virtual void Stop_Implementation(const FName& Name=NAME_None);

	UFUNCTION(BlueprintCallable)
	void Add(AStep* Step);

	UFUNCTION(BlueprintCallable)
	void Rem(const FName& Name);

	UFUNCTION(BlueprintCallable)
	const FName& GetCurrent();

	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FStoryStepStart OnStepStart;
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FStoryStepStop OnStepStop;
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FStorySeqStop OnSeqStop;
	
protected:
	bool StartNextStep();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TMap<FName, AStep*> Steps;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AStep* Current = nullptr;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TArray<FName> Sequence;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	int32 SeqStep = -1;
};

