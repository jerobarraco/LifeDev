// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "StoryTypes.h"

#include "Story.generated.h"

class AStep;
class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStop, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStart, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStorySeqStop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStoryFade, bool, In, const FText&, Text);

// World subsystem to deal with Inventory
UCLASS(Blueprintable, Category="Story")
class STORY_API UStory : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void DeInit();
	
	UFUNCTION(BlueprintCallable)
	bool StartSequence(const FText& Title, const TArray<FName>& InSeq);

	// starts a step by the name. stops the current one before that.
	UFUNCTION(BlueprintCallable)
	bool Start(const FName& Name);

	// stops a step. if not specified it will stop the current one. then it will start the next.
	// the name is a protection mechanism mostly, used by the story steps.
	UFUNCTION(BlueprintCallable)
	void Stop(const FName& Name=NAME_None);

	UFUNCTION(BlueprintCallable)
	void Add(AStep* Step);

	UFUNCTION(BlueprintCallable)
	void Rem(const FName& Name);

	UFUNCTION(BlueprintCallable)
	const FName& GetCurrent();

	// TODO to be used with the fade 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float FadeTime = 2;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FStoryStepStart OnStepStart;
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FStoryStepStop OnStepStop;
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FStorySeqStop OnSeqStop;
	// TODO to be triggered when a fade should occur. the story manager should fade the ui.
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FStoryFade OnFade;

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

	// TODO to be used to know if it needs to fade in
	bool IsFaded = false; 
};

