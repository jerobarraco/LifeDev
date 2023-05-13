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

	void Trigger() const;

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool IsEnabled);

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnTrigger OnTrigger;
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnHover OnHover;
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FText Text = FText::GetEmpty();

	// used by the interactor. don't call directly. subscribe to the OnHover delegate.
	void Hover(bool IsHover) {OnHover.Broadcast(IsHover);}
};
