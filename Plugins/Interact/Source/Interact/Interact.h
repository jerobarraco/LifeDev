// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "InteractTypes.h"
#include "Components/PostProcessComponent.h"

#include "Interact.generated.h"

class UCQuickMesh;
class UCAnimatorTrans;
class UCInteract;
class USoundBase;
class UAudioComponent;

// Don't use unless you really need it.
// It's better to use AInteractAnim and disable the animations.
// Base class for interact actors (actors to interact with)
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteract: public AActor {
	GENERATED_BODY()

public:
	AInteract();

	// Call this to trigger the interaction. Returns the success (false if locked)
	// this function has side-effects (calls trigger/trigger locked) so call at the end.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryTrigger();
	virtual bool TryTrigger_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Hover(bool IsOn);
	virtual void Hover_Implementation(bool IsOn);

	// returns true if the item has been used (notice past tense)
	//  this means when calling this function the item WILL trigger
	// Override and activate the item here.
	//  (but don't modify the inventory as part of this, and you wont be able since this plugin can't see the inventory)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EItemUseResult TryUseItem(const FName& Name);
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name);

	// used for binding only. since ue will complain about the return value. but i still wanna keep it.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact")
	void TryTriggerWrapped() {TryTrigger();}

	// enables or disables the interaction
	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool Enabled = true);

	// be careful with this. will set an actor mobility and its components too. Override this and also apply to every scene component (or child of) you have or the object will break on builds (but not PIE)
	UFUNCTION(BlueprintCallable)
	virtual void SetMobility(EComponentMobility::Type Mobility);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact")
	virtual void Reset() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	bool Locked = false;

	// SFX that will be played on trigger
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Trigger = nullptr;
	// SFX that will be played on trigger locked
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Locked = nullptr;

	// the state of the interact. will be used by the puzzle and the interactanim, but also you can use it however you want.
	UPROPERTY(BlueprintReadWrite)
	int32 State = 0;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void SetText();
	virtual void SetText_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Trigger();
	virtual void Trigger_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void TriggerLocked();
	virtual void TriggerLocked_Implementation();

	// test function.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interact", meta=(DeprecatedFunction))
	void SetInteractAutoBounds();

	UFUNCTION(BlueprintCallable)
	void PlaySFX(USoundBase* Snd);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCInteract* Interact = nullptr;
	
	// the root for animations, and positioning the mesh.
	// Don't change the transform of this guy. change the transform of the children.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* IRoot = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Mesh = nullptr;
	// Defined as QuickMesh so that child objects can access their properties/functions
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPostProcessComponent* PostProcess = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* SFX = nullptr;
};
