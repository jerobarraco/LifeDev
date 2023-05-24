// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/PostProcessComponent.h"

#include "Interact.generated.h"

class UCAnimator;
class UCInteract;

// Base class for interact actors
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteract: public AActor {
public:
	GENERATED_BODY()

	AInteract();

	UFUNCTION(BlueprintNativeEvent)
	void Trigger();
	virtual void Trigger_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Hover(bool IsOn);
	virtual void Hover_Implementation(bool IsOn);

	// returns true if the item has been used (notice past tense)
	// Override and activate the item here. dont modify the inventory.
	// will return if the item is usable, (and trigger action with custom code)
	// To be overriden on child objects
	UFUNCTION(BlueprintNativeEvent)
	bool TryUseItem(const FName& Name);
	virtual bool TryUseItem_Implementation(const FName& Name);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void SetText();
	virtual void SetText_Implementation();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetInteractAutoBounds();

	// the root for animations, and positioning the mesh.
	// Don't change the transform of this guy. change the transform of the children.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* IRoot = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCInteract* Interact = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPostProcessComponent* PostProcess = nullptr;
};
