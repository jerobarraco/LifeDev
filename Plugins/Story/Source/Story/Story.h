// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Story.generated.h"

class AStep;
class UDataTable;
// can't use *Const as it breaks AStoryMan due to ue bug
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStop, AStep *, Step);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoryStepStart, AStep *, Step);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStoryFade, const bool, In, const FText&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStoryDLChange, const FName&, Name, const bool, On, const bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStorySeqStop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStorySeqStart);

// World subsystem to deal with Story flow progression
UCLASS(Blueprintable, Category="Story", Config=Story, DefaultConfig)
class STORY_API UStory : public UWorldSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UStory* Instance(const UObject* const O);

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
	bool StartNext(const FName CurrentName = NAME_None);
	
	// force starts a step by the name (skips fading). stops the current one before that.
	// to use fading call Add(Step) then Start(Step.Name)
	UFUNCTION(BlueprintCallable)
	bool StartNow(AStep* const NewStep);

	// starts a step by the name. stops the current one before that
	// Stop is preferred to allow changes in the sequence since itś better that each step doesn't need to know about the others,
	// and when you change the story you'll be embedded in a step. So stop make more sense than start.
	UFUNCTION(BlueprintCallable)
	bool Start(const FName Name);

	// stops a step. if not specified it will stop the current one.
	UFUNCTION(BlueprintCallable)
	void Stop();

	// adds an AStep to be tracked and managed.
	UFUNCTION(BlueprintCallable)
	void Add(AStep* const Step);

	// Untracks/removes an AStep by name.
	UFUNCTION(BlueprintCallable)
	void Rem(const FName Name);

	// returns the corresponding tracked AStep by name. or nullptr otherwise.
	// if you want to check if a step exists, just call this function and check for isValid or non null.
	// a Has(Name) function is redundant and hides information.
	UFUNCTION(BlueprintCallable)
	AStep* GetStep(const FName Name);

	// returns the name of the current step
	UFUNCTION(BlueprintCallable)
	FName GetCurrent() const;

	// fades in/out with a text. nothing else.
	UFUNCTION(BlueprintCallable)
	void AutoFade(const FText& Title);

	// Set this from game instance or smth. Used for the timing on the steps. 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float FadeTime = 2;
	// Set this from game instance or smth. Used for the timing on the steps. 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float HoldTime = 2;

	// forces gc after a fade load (when a step has "UseFade")
	// it has the side effect that it will push the next gc.
	// so even if nothing is unloaded it will buy time till the next gc. 
	// https://youtu.be/HaVTYSnGvxA?t=2318
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	bool UseFadeGC = true;

	// triggered when a step starts. (just after Step->TryStart is called, probably before Step->Start)
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Events")
	FStoryStepStart OnStart;
	// triggers when a step stops. it's not triggered for the last step, until a new chapter is loaded. see note on StartNext
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
	bool ToggleLayer(const UDataLayerAsset* const DLA, bool On) const;
	bool ToggleStepLayers() const;

	// these are ALL the steps registered (i think)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TMap<FName, TObjectPtr<AStep>> Steps;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<AStep> Current = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TArray<FName> Sequence;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	int32 SeqStep = -1;
};

