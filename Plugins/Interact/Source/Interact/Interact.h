// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Interact.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UCInteractor;
class UPhysicsConstraintComponent;
class UCQuickMesh;
class UCAnimatorTrans;
class UCInteract;
class USoundBase;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAInteractOnHover, bool, IsOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAInteractOnHint);

// Base class for interactable actors (actors to interact with)
// Override DoTrigger and DoTriggerLocked, maybe OnHover.
// And check the properties under "SetUp".
// By default, they start inactive (See CInteract)
UCLASS(Blueprintable, BlueprintType, Config=Interact, DefaultConfig)
class INTERACT_API AInteract: public AActor {
	GENERATED_BODY()

public:
#pragma region base
	AInteract();
	// will also SetActive if UseAutoActivate or hidden is true.
	// will not SetActive if NewHidden is false (when showing back).
	// during constructor will set UseAutoActivate instead.
	virtual void SetActorHiddenInGame(const bool NewHidden) override;
#pragma endregion

	// Will attempt to trigger the interaction. can be blocked by internal flags (locked)
	// Call this to trigger the interaction. Returns the success (false if locked)
	// this function has side effects (calls trigger/triggerLocked) so call at the end of your function.
	// Usually this gets called automatically by the Interactor/CInteract
	// overrideable in case you need to cancel a trigger,
	// but it's better to override DoTrigger and/or DoTriggerLocked.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact", meta=(ForceAsFunction))
	bool TryTrigger();

	// Don't use if possible. use TryTrigger. This is used for binding only,
	// since ue will complain about the return value.
	// bindings don't work with forceinline
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact", meta=(AdvancedDisplay))
	void TryTriggerWrap() {TryTrigger();}

	// will force trigger the Interact, even if locked (will call DoTrigger instead of DoTriggerLocked). used mostly for other automations.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact", meta=(AdvancedDisplay))
	FORCEINLINE void TriggerForced() {Trigger();};
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact", meta=(ForceAsFunction))
	void Hover(const bool IsOn);
	virtual void Hover_Implementation(const bool IsOn) {
		OnHover.Broadcast(IsOn);
	}

	// Enables or disables the interaction. Will not fade.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact", meta=(ForceAsFunction))
	void SetActive(const bool Active = true);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact")
	bool GetActive() const;

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
	// skips if out of bounds.
	// If StateNum ==1 this will might get called with the same state as current (might change, but probably not)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact", meta=(ForceAsFunction))
	void SetState(const int32 NewState);

	// this is _similar_ to SetState. it's meant to be called from the constructor, or when a state needs to be set instantly, or skipping aspects.
	// but mostly constructor, the rest is side effect, and not a priority.
	// i'm pretty sure NativeEvents and constructor will not call the child classes.
	// if that fails, then use PostLoad or smth.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact", meta=(ForceAsFunction))
	void SetStateNow(const int32 NewState, const bool UseSFX = false, const bool UseParts = false);

	// this function has no documentation, oh noes, is so complicated i can't even
	// begin to describe it. too bad.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetState() const { return State; }

	// returns the "label" for this interact actor.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetLabel() const { return Label; }
 
#pragma region Hint
	// test function to hint the interact (call attention to it). atm it will trigger Hover.
	// triggers OnHint, and uses the Anim subsystem (optionally).
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction), Category="Hint")
	bool ShowHint();
	// this should still allow to set useHint on the editor and constructor
	// this is virtual so that the puzzles can pass this to its pieces.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction), Category="Hint")
	void SetUseHint(const bool NewHint=true);
	virtual void SetUseHint_Implementation(const bool NewHint=true) { UseHint = NewHint; }

	// whether to disable hints on trigger.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Hint")
	bool UseTriggerDeHint = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Hint", Config)
	float HintTime = 5;

	// experimental. Condition that needs to be >=1 for hint to work.
	// Try to use RewardInterHint on another interact or step. it'll be faster than this.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Hint", AssetRegistrySearchable)
	FString HintCondition;

	// when >= 0 it will also animate the custom primitive data index=ID, from 0 to 1.
	// requires the Anim subsystem to be created (set ShouldCreate to true on config)
	// this is a quick way to disable CustomPrimData for this specific item
	// or in general by setting it on the config.
	// if this is <0 then no prim data animation is done. BUT showHint will still return true (when it succeeds).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Hint", Config)
	int32 HintPrimDataID = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Hint", Config)
	TSoftObjectPtr<UCurveFloat> HintCurve = nullptr;
	// ^ needs to be a soft ptr due to "config"

	// triggered on showhint(true)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Hint")
	TObjectPtr<USoundBase> SFXHint = nullptr;
#pragma endregion
#pragma region Lock
	// Will CHECK if it needs to unlock (due to a condition, or whatever)
	// Called automatically on TryTrigger (and maybe other places in the future, like in the puzzle)
	// return true if it should unlock. it does not unlock. but trytrigger will call unlock if necessary.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	bool ShouldUnlock();
	// Called to unlock, or when unlocked. triggers a delegate and dialogs and flags and sparks and whistles.
	// if you override this, make sure to check for Locked before, to avoid double triggering.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Unlock();
	// locks the interaction, calling tryTrigger will return false.
	// But it will execute TriggerLocked and play the locked sound.
	// You can change this during runtime whenever you want, but calling Unlock() is preferred.
	// Also check 'IsOneShot'.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	bool IsLocked = false;

	// An eval condition that will unlock this interact.
	//	E.g. On a flag: {flag}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock", AssetRegistrySearchable)
	FString UnlockCondition = "";
	// SFX that will be played on trigger locked
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	TObjectPtr<USoundBase> SFXLocked = nullptr;
	// TODO i could have a sfxUnlock but i don't really need it just now. otherwise take a look at doorI10
#pragma endregion
#pragma region Setup
	// An interact id used for auto dialogs and such.
	// This is to overcome the issue with GetActorLabel not working on packaged builds ò_ó
	// It will default to the actor label. It will get updated when changed on the editor.
	// if this is an instance that it's spawned, it will default to the Name.
	// this property is defined here so that it gets packaged correctly, which is not what ActorLabel does. mottainai.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base", AssetRegistrySearchable)
	FName Label;
	// don't use advanced, or it won't show since i have a subcategory (base)

	// whether to de/activate on BeginPlay and SetActorHiddenInGame.
	// (don't setting SetAutoActivate on the Interact component!).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	bool UseAutoActivate = false;
	// When true will disable the interact on trigger. Calling Deactivate.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	bool IsOneShot = false;
	// whether to use the attached SFX component or just spawn a "sound at location".
	// A subclass changes this to allow for playing sounds when destroying.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="SetUp|SFX")
	bool UseAttachedSFX = true;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base", AssetRegistrySearchable, meta=(MultiLine))
	FString Comment;
#endif
#pragma region State
	// Number of states. It will wrap State if UseStateLoop is set. Otherwise, it will clamp at StateNum-1.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|State")
	int32 StateNum = 2;
	// Loops the states (and the texts). Means when it reaches the last one. it goes back to the first.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SetUp|State")
	bool UseStateLoop = true;

	// Text to be displayed on each state. Closed, Open.
	// Make sure it matches StateNum.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|State")
	TArray<FText> Texts = {
		NSLOCTEXT("Interact", "State0", "Open"), // 0 == !IsOpen == Closed text
		NSLOCTEXT("Interact", "State1", "Close"), // 1 == IsOpen == Opened text
	};
	// particles to emit on state change. index matches state.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|State")
	TArray<TObjectPtr<UNiagaraSystem>> Particles;
	// SFX that will play on state change (regardless of trigger/trigger locked/manual). Index matches the state.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|State")
	TArray<TObjectPtr<USoundBase>> SFXs;

#pragma endregion
#pragma endregion
#pragma region Rewards
	// Interacts to set UseHint when this is triggered.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	TArray<TSoftObjectPtr<AInteract>> RewardIntersHint;

	// interacts to trigger when this interact is triggered.
	// It still triggers an interact even if it's not "Active".
	// Note: This just calls Trigger. If the other one is locked it will call TriggerLocked.
	// And if the other one has different number of states they might get out of "sync" with this.
	// Which is not bad, but keep it in mind.
	// The upside is, that you can have two switches controlling the same light with this behavior.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	TArray<TSoftObjectPtr<AInteract>> RewardIntersTrigger;

	// interacts to activate when this interact is triggered.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	TArray<TSoftObjectPtr<AInteract>> RewardIntersActive;
#pragma endregion
#pragma region Delegates
	// When this is triggered (not locked).
	// Either you override DoTrigger or you subscribe to this, but unlikely both.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnTrigger OnTrigger;

	// When this is triggered while locked.
	// Can also override DoTriggerLocked.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnTrigger OnTriggerLocked;

	// When this is being hovered on/off
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnHover OnHover;
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnHint OnHint;
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;
	virtual void PostActorCreated() override;

	// Will attempt to grab the interaction. can be blocked by internal flags (isGrabbable)
	// Returns the success (false if locked)
	// this function has side effects (calls doGrabbed/doUnGrabbed) so call at the end of your function.
	// these are called by the CInteract which is called by the CInteractor.
	// This is quite a complex interaction that's why it's protected.
	UFUNCTION()
	void Grab(const bool IsGrab, UCInteractor* const NewParent);

	// Set the current text on the CInteract. Called on SetState, BeginPlay and by SetStateNow.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Interact, meta=(ForceAsFunction))
	void SetText();

	// called when the object actually gets triggered. and dispatches the delegate.
	// TryTrigger is preferred. unless you want to skip the checks.
	FORCEINLINE void Trigger() {
		DoTrigger();
		// at end, outside the overrideable function
		// so that i'm sure that children are done.
		OnTrigger.Broadcast();
	}
	
	// called when the object actually gets triggered while locked. and dispatches the delegate.
	// TryTrigger is preferred. unless you want to skip the checks.
	FORCEINLINE void TriggerLocked() {
		DoTriggerLocked();
		// at end, outside the overrideable function
		// so that i'm sure that children are done.
		OnTriggerLocked.Broadcast();
	}

	// called when the object is triggered.
	// override if you need to change the logic for the triggering. or when trigger but not reset.
	// otherwise setState is much more preferred.
	UFUNCTION(BlueprintNativeEvent, Category=Interact, meta=(ForceAsFunction))
	void DoTrigger();
	
	// called when an attempt to trigger happened while locked.
	// Override if you need to do something then.
	UFUNCTION(BlueprintNativeEvent, Category=Interact, meta=(ForceAsFunction))
	void DoTriggerLocked();

	// test function.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact", meta=(DeprecatedFunction))
	void SetInteractAutoBounds();

	// plays a sound using the SFX object.
	// Unless UseAttachedSFX is false, in which case it plays a sound at the location of the sfx object.
	UFUNCTION(BlueprintCallable, Category=Interact)
	void PlaySFX(USoundBase* const Snd) const;
	UFUNCTION(BlueprintCallable, Category=Interact)
	void PlayParts(UNiagaraSystem* const Part) const;
	void InitLabel();

	FORCEINLINE void EditorLabelUpd(AActor* const Actor);

	// The state (index) of the Interact.
	// it increases with every trigger. wraps by stateNum. so it's 0<=State<StateNum
	// will be used by the puzzle and the interactanim, but also you can use it however you want.
	// I recommend to use SetState and SetStateNow.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="SetUp|State")
	int32 State = 0;

	// whether to show hints or not. Check "UseHintCondition", and "UseTriggerDeHint".
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Hint")
	bool UseHint = false;

#pragma region cdo
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
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UNiagaraComponent> Emitter = nullptr;
#pragma endregion
};
