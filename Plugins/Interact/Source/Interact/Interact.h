// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/PostProcessComponent.h"

#include "Interact.generated.h"

class UCInteract;

// Base class for interact actors
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteract: public AActor {
public:
	GENERATED_BODY()

	AInteract(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent)
	void Trigger();
	virtual void Trigger_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Hover(bool IsOn);
	virtual void Hover_Implementation(bool IsOn);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void SetText();
	virtual void SetText_Implementation();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetInteractAutoBounds();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* IRoot = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCInteract* Interact = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPostProcessComponent* PostProcess = nullptr;
};
