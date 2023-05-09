// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "UCInteract.generated.h"


class UBoxComponent;
// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCInteract: public USceneComponent {
public:
	GENERATED_BODY()

	UCInteract(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere )
	UBoxComponent* Collider = nullptr;
};
