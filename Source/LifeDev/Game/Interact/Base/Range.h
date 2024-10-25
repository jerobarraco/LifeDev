// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Interact/InteractAnim.h"

#include "Range.generated.h"

class UCInteract;
class UCAnimatorTrans;
class UCRange;

// Base class for Range actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARange: public AInteractAnim {
public:
	GENERATED_BODY()

	ARange();

	// lamest lame thing. to test in editor
	UFUNCTION(BlueprintCallable)
	void ETrigger() { Trigger(); }

	UFUNCTION(BlueprintCallable)
	void SetMaxSize(float Size);

	virtual void DoTrigger_Implementation() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UMaterialInterface> Mat = nullptr;

protected:
	virtual void BeginPlay() override;
};
