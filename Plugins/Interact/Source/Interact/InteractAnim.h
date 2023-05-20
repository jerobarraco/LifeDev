// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Interact/Interact.h"

#include "InteractAnim.generated.h"

// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteractAnim: public AInteract {
public:
	GENERATED_BODY()

	AInteractAnim(const FObjectInitializer& ObjectInitializer);

	virtual void Trigger_Implementation() override;
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Opened Text")), // isOpen
		FText::FromString(TEXT("Closed text")), // !IsOpen
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UCurveFloat* AnimCurve = nullptr;

	// Duration of the animation in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float AnimDuration = 2.0;
	
	// Duration of the animation in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool IsAutoTick = true;
	
	// The animation transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FTransform TAnim;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DT) override;
	virtual void SetText_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void SetIsAnimating(bool NewIsRotating);
	// starts closed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsOpen = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsAnimating = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float AnimProgress = 0.0;
	// Reference rotations
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FTransform TClosed;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FTransform TOpen;
	// Relative rotations for the animation
	// at the start of the animation
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FTransform TStart;
};
