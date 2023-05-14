// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Interact.h"

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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Close the door")), // isOpen
		FText::FromString(TEXT("Open the door")), // !IsOpen
	};

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	UCurveFloat* RotCurve = nullptr;

	// Duration of the animation
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float RotDuration = 2.0;

	// the door rotations. Open and closed.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<float> Rots = { -90, 0 };

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DT) override;
	virtual void SetText_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void SetIsRotating(bool NewIsRotating);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsOpen = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsRotating = false;
	// Rotation at the start of the lifecycle
	float RotStart = 0;
	float RotStop = 0;
	// Rotation at beginplay
	FRotator RotBegin = FRotator::ZeroRotator;
	float RotProgress = 0.0;
};
