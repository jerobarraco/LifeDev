// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "InteractTypes.h"

#include "Interact.generated.h"

class UPhysicsConstraintComponent;
class UCQuickMesh;
class UCAnimatorTrans;
class UCInteract;
class USoundBase;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnTryTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnTriggerLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAInteractOnHover, bool, IsOn);

// Base class for interactable actors (actors to interact with)
// Override DoTrigger and DoTriggerLocked, maybe OnHover.
// And check the properties under "SetUp".
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteract: public AActor {
	GENERATED_BODY()

public:
	AInteract();

	// Will attempt to trigger the interaction. can be blocked by internal flags (locked)
	// Call this to trigger the interaction. Returns the success (false if locked)
	// this function has side-effects (calls trigger/triggerLocked) so call at the end of your function.
	// Usually this gets called automatically by the Interactor/CInteract
	// overrideable in case you need to cancel a trigger
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	bool TryTrigger();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void Hover(bool IsOn);
	virtual void Hover_Implementation(bool IsOn) {
		OnHover.Broadcast(IsOn);
	};

	// returns true if the item has been used (notice past tense)
	//  this means when calling this function the item WILL trigger
	// Override and activate the item here.
	//  (but don't modify the inventory as part of this! (and you won't be able since this plugin can't see the inventory).
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	EItemUseResult TryUseItem(const FName& Name);
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name);

	// Don't use if possible. use TryTrigger. This is used for binding only,
	// since ue will complain about the return value.
	// bindings don't work with forceinline
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact", meta=(AdvancedDisplay))
	void TryTriggerWrap() {TryTrigger();}

	// Enables or disables the interaction.
	UFUNCTION(BlueprintNativeEvent, Category="Interact")
	void SetActive(const bool Active = true);
	UFUNCTION(BlueprintCallable, Category="Interact")
	virtual void SetAutoActivate(const bool AutoActive = true);
	
	// this CAN NOT be BlueprintNativeEvent because
	// it breaks on the constructor for some extremely weird reason i don't know of yet.
	// and THIS function is called in the constructor everywhere.
	// mostly debug.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact")
	bool GetEnabled() const;

	// be careful with this. will set an actor mobility and its components too. Override this and also apply to every scene component (or child of) you have or the object will break on builds (but not PIE)
	UFUNCTION(BlueprintCallable)
	virtual void SetMobility(const EComponentMobility::Type Mobility);

	// mostly to support the pooling system.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact")
	virtual void Reset() override;

	// called on trigger or state reset.
	// override it to do something when it's triggered or reset.
	// call it to change the state without triggering.
	// called when the state changes because it triggered.
	// Starts at state 0 == closed == off
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void SetState(const int32 NewState);
	virtual void SetState_Implementation(const int32 NewState);

	// this function has no documentation, oh noes, is so complicated i can't even
	// begin to describe it. too bad.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetState() const { return State; }

	// locks the interaction, calling tryTrigger will return false.
	// But it will execute TriggerLocked and play the locked sound.
	// You can change this during runtime whenever you want. Also check 'IsOneShot'.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock",
		Replicated, ReplicatedUsing=OnRep_IsLocked)
	bool Locked = false;

	// When true will disable the interact on trigger. Calling Deactivate.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp",
		Replicated, ReplicatedUsing=OnRep_IsOneShot)
	bool IsOneShot = false;
	
	// whether to use the attached SFX component or just spawn a "sound at location".
	// A subclass changes this to allow for playing sounds when destroying.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="SetUp|SFX")
	bool UseAttachedSFX = true;

	// SFX that will be played on trigger
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	TObjectPtr<USoundBase> SFX_Trigger = nullptr;
	// SFX that will be played on trigger locked
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	TObjectPtr<USoundBase> SFX_Locked = nullptr;

	// When this is triggered (not locked).
	// Either you override DoTrigger or you subscribe to this, but unlikely both.
	// also Overriding DoTrigger is not the best.
	// If the CInteract is replicated, this will execute only on server.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnTrigger OnTrigger;

	// When this is triggered while locked.
	// Can also override DoTriggerLocked.
	// If the CInteract is replicated, this will execute only on server.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnTriggerLocked OnTriggerLocked;

	// When this is being hovered on/off
	// Always executes on the client that triggered this,
	// The triggering CInteractor is always an AutonomousProxy not a SimulatedProxy.
	// Though THIS Interact could be a SimulatedProxy. 
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnHover OnHover;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Will attempt to grab the interaction. can be blocked by internal flags (isGrabbable)
	// Returns the success (false if locked)
	// this function has side-effects (calls doGrabbed/doUnGrabbed) so call at the end of your function.
	// these are called by the CInteract which is called by the CInteractor.
	// This is quite a complex interaction that' s why it' s protected.
	UFUNCTION()
	void Grab(const bool IsGrab, UCInteractor* const NewParent);

	// sets the current text to show on this interact
	UFUNCTION(BlueprintNativeEvent, Category=Interact)
	void SetText();
	virtual void SetText_Implementation(){}

	// called when the object actually gets triggered. and dispatches the delegate.
	// TryTrigger is preferred. unless you want to skip the checks.
	UFUNCTION(BlueprintCallable, Category=Interact)
	FORCEINLINE void Trigger() {
		DoTrigger();
		// at end, outside the overrideable function
		// so that i'm sure that children are done.
		OnTrigger.Broadcast();
	}
	
	// called when the object actually gets triggered while locked. and dispatches the delegate.
	// TryTrigger is preferred. unless you want to skip the checks.
	UFUNCTION(BlueprintCallable, Category=Interact)
	FORCEINLINE void TriggerLocked() {
		DoTriggerLocked();
		// at end, outside the overrideable function
		// so that i'm sure that children are done.
		OnTriggerLocked.Broadcast();
	}
	
	// called when the object is triggered.
	// override if you need to change the logic for the triggering. or when trigger but not reset.
	// otherwise setState is much more preferred.
	UFUNCTION(BlueprintNativeEvent, Category=Interact)
	void DoTrigger();
	
	// called when an attempt to trigger happened while locked.
	// Override if you need to do something then.
	UFUNCTION(BlueprintNativeEvent, Category=Interact)
	void DoTriggerLocked();

	// test function.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact", meta=(DeprecatedFunction))
	void SetInteractAutoBounds();

	// plays a sound using the SFX object.
	// Unless UseAttachedSFX is false, in which case it plays a sound at the location of the sfx object.
	UFUNCTION(BlueprintCallable, Category=Interact, NetMulticast, Reliable)
	void PlaySFX(USoundBase* Snd) const; // native events can't take Ptr* const
	
	UFUNCTION(BlueprintNativeEvent, Category="Interact|Rep")
	void OnRep_IsLocked();

	UFUNCTION(BlueprintNativeEvent, Category="Interact|Rep")
	void OnRep_IsOneShot();

	// The state (index) of the Interact.
	// it increases with every trigger. wraps by stateNum. so it's 0<=State<StateNum
	// will be used by the puzzle and the interactanim, but also you can use it however you want.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SetUp|State")
	int32 State = 0;

	// Number of states. It will wrap State around. around.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|State")
	int32 StateNum = 2;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	TArray<TObjectPtr<AInteract>> RewardInterEnable;

	/// CDO

	// added here, so it can be changed in the editor. otherwise it, won't show. :(
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Root = nullptr;

	// handles the interactions with this actor.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCInteract> Interact = nullptr;
	// the root for animations, and positioning the mesh.
	// Don't change the transform of this guy. change the transform of the children.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> IRoot = nullptr;
	// default mesh
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCQuickMesh> Mesh = nullptr;
	// Defined as QuickMesh so that child objects can access their properties/functions
	
	// default sfx player. Use PlaySFX 
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UAudioComponent> SFX = nullptr;
	// cant be a clsounder since this is the plugin
};
