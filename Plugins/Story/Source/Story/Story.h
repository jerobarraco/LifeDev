// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "StoryTypes.h"

#include "Story.generated.h"

class AStep;
class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStop, AStep*, Step);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStart, AStep*, Step);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStoryFade, bool, In, const FText&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStoryDLChange, const FName&, Name, bool, On, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStorySeqStop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStorySeqStart);

// World subsystem to deal with Story flow progression
UCLASS(Blueprintable, Category="Story")
class STORY_API UStory : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UStory* Instance(UWorld* W);
	
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void DeInit();

	// starts a sequence. if one is already running it will override it.
	UFUNCTION(BlueprintCallable)
	bool StartSequence(const TArray<FName>& InSeq);

	// The main function to call.
	// Stops the current step and starts the next one.
	// the name is a protection mechanism mostly, used by the story steps.
	UFUNCTION(BlueprintCallable)
	bool StartNext(const FName& CurrentName = NAME_None);
	
	// force starts a step by the name (skips fading). stops the current one before that.
	UFUNCTION(BlueprintCallable)
	bool StartNow(AStep* NewStep);

	// starts a step by the name. stops the current one before that
	// Stop is preferred to allow changes in the sequence since itś better that each step doesn't need to know about the others,
	// and when you change the story you'll be embedded in a step. So stop make more sense than start.
	UFUNCTION(BlueprintCallable)
	bool Start(const FName& Name);

	// stops a step. if not specified it will stop the current one.
	UFUNCTION(BlueprintCallable)
	void Stop();
	
	UFUNCTION(BlueprintCallable)
	void Add(AStep* Step);

	UFUNCTION(BlueprintCallable)
	void Rem(const FName& Name);

	UFUNCTION(BlueprintCallable)
	AStep* GetStep(const FName& Name);

	UFUNCTION(BlueprintCallable)
	const FName& GetCurrent();

	// fades in/out with a text. nothing else.
	UFUNCTION(BlueprintCallable)
	void AutoFade(const FText& Title);

	// Set this from game instance or smth TODO to be used with the fade 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float FadeTime = 2;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float HoldTime = 2;

	// triggered when a step starts
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Events")
	FStoryStepStart OnStart;
	// triggers when a step stops
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Events")
	FStoryStepStop OnStop;
	// triggered when a fade should occur. the story manager should fade the ui.
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Events")
	FStoryFade OnFade;
	// triggered when a sequence starts
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Events")
	FStorySeqStop OnSeqStart;
	// triggered when a sequence stops
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Events")
	FStorySeqStop OnSeqStop;
	// triggers *after* a data layer has been loaded/unloaded
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Events")
	FStoryDLChange OnDlChange;

protected:
	bool ToggleDataLayer(const UDataLayerAsset* DLA, bool On) const;
	bool ToggleStepLayers() const;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TMap<FName, AStep*> Steps;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AStep* Current = nullptr;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TArray<FName> Sequence;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	int32 SeqStep = -1;
};

