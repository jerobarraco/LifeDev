// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "InteractAnim.h"

#include "InteractHolder.generated.h"

// An interactive actor that can have an animation and hold an item
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteractHolder: public AInteractAnim {
public:
	GENERATED_BODY()

	AInteractHolder();

	// The animation transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FTransform ItemTrans;

	// The animation transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TSubclassOf<AActor> ItemClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UChildActorComponent* Item;
};
