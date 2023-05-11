// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/BoxComponent.h"

#include "CInteract.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractOnTrigger);
// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCInteract: public UBoxComponent {
public:
	GENERATED_BODY()

	UCInteract(const FObjectInitializer& ObjectInitializer);

	void Trigger() const;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnTrigger OnTrigger;
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FText Text = FText::GetEmpty();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UBoxComponent* Collider = nullptr;
};
