// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/PostProcessComponent.h"

#include "Interact.generated.h"

class UCAnimatorTrans;
class UCInteract;

// Base class for interact actors
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteract: public AActor {
public:
	GENERATED_BODY()

	AInteract();

	// Call this to trigger the interaction. will check if it's locked.
	UFUNCTION(BlueprintNativeEvent)
	bool TryTrigger();
	virtual bool TryTrigger_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Hover(bool IsOn);
	virtual void Hover_Implementation(bool IsOn);

	// returns true if the item has been used (notice past tense)
	// Override and activate the item here, but don't modify the inventory.
	// will return if the item is usable, (and trigger action with custom code)
	// To be overriden on child objects
	UFUNCTION(BlueprintNativeEvent)
	bool TryUseItem(const FName& Name);
	virtual bool TryUseItem_Implementation(const FName& Name);

	// used for binding only. since ue will complain about the return value. but i still wanna keep it.
	UFUNCTION(BlueprintCallable)
	void TryTriggerWrapped() {TryTrigger();}

	UPROPERTY(BlueprintReadWrite)
	bool Locked = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCInteract* Interact = nullptr;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void SetText();
	virtual void SetText_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Trigger();
	virtual void Trigger_Implementation();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetInteractAutoBounds();

	// the root for animations, and positioning the mesh.
	// Don't change the transform of this guy. change the transform of the children.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* IRoot = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPostProcessComponent* PostProcess = nullptr;
};
