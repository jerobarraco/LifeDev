// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "InteractTypes.h"
#include "Components/PostProcessComponent.h"

#include "Interact.generated.h"

class UCAnimatorTrans;
class UCInteract;
class USoundBase;
class UAudioComponent;

// Don't use unless you really need it.
// It's better to use AInteractAnim and disable the animations.
// Base class for interact actors (actors to interact with)
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteract: public AActor {
public:
	GENERATED_BODY()

	AInteract();

	// Call this to trigger the interaction. Returns the success (false if locked)
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
	UFUNCTION(BlueprintCallable)
	void TryTriggerWrapped() {TryTrigger();}

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool Enabled);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	bool Locked = false;

	// SFX that will be played on trigger
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Trigger = nullptr;
	// SFX that will be played on trigger locked
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Locked = nullptr;

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
	
	UFUNCTION(BlueprintCallable, CallInEditor)
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
	UStaticMeshComponent* Mesh = nullptr;
	// not defining it as UCQuickMesh since it doesn't add anything. but it will make other files have to import quickmesh
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPostProcessComponent* PostProcess = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* SFX = nullptr;
};
