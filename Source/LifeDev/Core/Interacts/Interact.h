// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "Interact.generated.h"

class UCInteract;

// Base class for interact actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AInteract: public AActor {
public:
	GENERATED_BODY()

	AInteract(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent)
	void Trigger();
	virtual void Trigger_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Hover(bool IsOn);
	virtual void Hover_Implementation(bool IsOn);

	// Mesh to automatically highlight
	UPROPERTY(BlueprintReadWrite, Category=SetUP)
	UStaticMeshComponent* HoverMesh = nullptr;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void SetText();
	virtual void SetText_Implementation();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetInteractAutoBounds();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCInteract* Interact = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
};
