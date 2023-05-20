// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Interact/Interact.h"

#include "LDoor.generated.h"

class UCInteract;

// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALDoor: public AInteract {
public:
	GENERATED_BODY()

	ALDoor(const FObjectInitializer& ObjectInitializer);

	virtual void Trigger_Implementation() override;
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Close the door")), // isOpen
		FText::FromString(TEXT("Open the door")), // !IsOpen
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UCurveFloat* RotCurve = nullptr;

	// Duration of the animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotDuration = 2.0;

	// the door rotations. Open and closed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rot = {0,-90, 0};

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DT) override;
	virtual void SetText_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void SetIsRotating(bool NewIsRotating);
	// starts closed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsOpen = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsRotating = false;
	// Reference rotations
	FRotator RotClosed = FRotator::ZeroRotator;
	FRotator RotOpen = FRotator::ZeroRotator;
	// Relative rotations for the animation
	// at the start of the animation
	FRotator RotStart = FRotator::ZeroRotator;
	// The target delta to apply
	FRotator RotDelta = FRotator::ZeroRotator;
	float RotProgress = 0.0;
};
