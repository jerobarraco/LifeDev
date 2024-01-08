// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "InteractTypes.h"

#include "Interact.generated.h"

class UCQuickMesh;
class UCAnimatorTrans;
class UCInteract;
class USoundBase;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnTryTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnTriggerLocked);

// Don't use unless you really need it.
// It's better to use AInteractAnim and disable the animations.
// Base class for interact actors (actors to interact with)
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteract: public AActor {
	GENERATED_BODY()

public:
	AInteract();

	// Will attempt to trigger the interaction. can be blocked by internal flags (locked)
	// Call this to trigger the interaction. Returns the success (false if locked)
	// this function has side-effects (calls trigger/triggerLocked) so call at the end of your function.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	bool TryTrigger();
	virtual bool TryTrigger_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void Hover(bool IsOn);
	virtual void Hover_Implementation(bool IsOn){};

	// returns true if the item has been used (notice past tense)
	//  this means when calling this function the item WILL trigger
	// Override and activate the item here.
	//  (but don't modify the inventory as part of this, and you wont be able since this plugin can't see the inventory)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	EItemUseResult TryUseItem(const FName& Name);
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name);

	// Don't use if possible. use TryTrigger. used for binding only. since ue will complain about the return value. but i still wanna keep it. 
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact")
	void TryTriggerWrapped() {TryTrigger();}

	// enables or disables the interaction
	UFUNCTION(BlueprintCallable, Category="Interact")
	void SetEnabled(bool Enabled = true);

	// be careful with this. will set an actor mobility and its components too. Override this and also apply to every scene component (or child of) you have or the object will break on builds (but not PIE)
	UFUNCTION(BlueprintCallable)
	virtual void SetMobility(EComponentMobility::Type Mobility);

	// mostly to support the pooling system.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact")
	virtual void Reset() override;

	// called on trigger or state reset.
	// override it to do something when it's triggered or reset.
	// call it to change the state without triggering.
	// called when the state changes because it triggered.
	// Starts at state 0 == closed == off
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void SetState(int32 NewState);
	virtual void SetState_Implementation(int32 NewState);

	// this function has no documentation, oh noes, is so complicated i can't even
	// begin to describe it. too bad.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetState() const { return State; }

	// locks the interaction, calling tryTrigger will return false
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	bool Locked = false;

	// SFX that will be played on trigger
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Trigger = nullptr;
	// SFX that will be played on trigger locked
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Locked = nullptr;

	// When this is triggered (not locked)
	// Beware that this will trigger just before the children implementations of this class are finished processing.
	// after long deliberation i think this is the best. either you override Trigger or you subscribe to this, but unlikely both.
	// also Overriding Trigger is not the best, SetState is preferred.
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category=SetUp)
	FAInteractOnTrigger OnTrigger;
	// When this is triggered while locked
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category=SetUp)
	FAInteractOnTriggerLocked OnTriggerLocked;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// sets the current text to show on this interact
	UFUNCTION(BlueprintNativeEvent, Category=Interact)
	void SetText();
	virtual void SetText_Implementation(){}

	// called when the object actually gets triggered
	// override if you need to change the logic for the triggering. or when trigger but not reset.
	// otherwise setState is much more preferred.
	// it will trigger OnTrigger at the end (which could be hard to time on children)
	// another reason to prefer SetState
	UFUNCTION(BlueprintNativeEvent, Category=Interact)
	void Trigger();
	virtual void Trigger_Implementation();
	// internal. used only so that the OnTrigger signal is ensured to be at the end.
	void TriggerWrapped();
	
	// called when an attempt to trigger happened while locked.
	// Override if you need to do something then.
	UFUNCTION(BlueprintNativeEvent, Category=Interact)
	void TriggerLocked();
	virtual void TriggerLocked_Implementation();

	// test function.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact", meta=(DeprecatedFunction))
	void SetInteractAutoBounds();

	// plays a sound using the SFX object.
	// Unless UseAttachedSFX is false, in which case it plays a sound at the location of the sfx object.
	UFUNCTION(BlueprintCallable, Category="Interact")
	void PlaySFX(USoundBase* Snd);
	
	// the state of the interact.
	// it increases with every trigger. wraps by stateNum. so it's 0<=State<StateNum
	// will be used by the puzzle and the interactanim, but also you can use it however you want.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=SetUp)
	int32 State = 0;

	// Number of states. It will wrap State around. around.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	int32 StateNum = 2;
	
	// whether to use the attached SFX component or just spawn a "sound at location".
	// A subclass changes this to allow for playing sounds when destroying.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=SetUp)
	bool UseAttachedSFX = true;

	// added here so it can be changed in the editor. otherwise it wont show. :(
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* Root = nullptr;
	
	// handles the interactions with this actor.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCInteract* Interact = nullptr;
	
	// the root for animations, and positioning the mesh.
	// Don't change the transform of this guy. change the transform of the children.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	USceneComponent* IRoot = nullptr;

	// default mesh
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCQuickMesh* Mesh = nullptr;
	// Defined as QuickMesh so that child objects can access their properties/functions

	// default sfx player. Use PlaySFX 
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UAudioComponent* SFX = nullptr;
};
