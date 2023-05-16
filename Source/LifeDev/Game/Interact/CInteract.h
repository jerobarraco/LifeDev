// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/BoxComponent.h"

#include "CInteract.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractOnHover, bool, IsOn);

// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCInteract: public UBoxComponent {
public:
	GENERATED_BODY()

	UCInteract(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool IsEnabled);

	// used by the interactor. don't call directly. subscribe to OnTrigger.
	UFUNCTION()
	void Trigger() const;
	
	// used by the interactor. don't call directly. subscribe to the OnHover delegate.
	UFUNCTION()
	void Hover(bool IsHover) const;

	UFUNCTION()
	void DeInit();
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnTrigger OnTrigger;
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnHover OnHover;
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FText Text = FText::GetEmpty();

	// Mesh to automatically highlight, if any.
	UPROPERTY(BlueprintReadWrite, Category=SetUP)
	UStaticMeshComponent* HoverMesh = nullptr;	
};
