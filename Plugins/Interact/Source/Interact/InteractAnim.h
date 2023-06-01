// Copyright (C) 2023 - Jerónimo Barraco-Mármol
#pragma once
#include "Interact/Interact.h"

#include "InteractAnim.generated.h"

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteractAnim: public AInteract {
public:
	GENERATED_BODY()

	AInteractAnim();

	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Opened Text")), // isOpen
		FText::FromString(TEXT("Closed text")), // !IsOpen
	};

protected:
	virtual void SetText_Implementation() override;
	virtual void Trigger_Implementation() override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimatorTrans* Animator = nullptr;

	// starts closed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsOpen = false;
};
