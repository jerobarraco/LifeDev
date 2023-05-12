// Copyright Jerónimo Barraco-Mármol

#include "LDoor.h"

#include "LifeDev/Core/Interacts/CInteract.h"

ALDoor::ALDoor(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	Interact->SetRelativeTransform(FTransform(
		FRotator::ZeroRotator,
		FVector(25,80,125),
		FVector::OneVector
	));
}

void ALDoor::BeginPlay() {
	Super::BeginPlay();
	RotBegin = GetActorRotation();
}

void ALDoor::Tick(float DT) {
	Super::Tick(DT);
	if (!IsRotating) return;

	const float ndt = DT/RotDuration;
	RotProgress += ndt;

	FRotator NewRot(RotBegin);

	const float Alpha = IsValid(RotCurve)? RotCurve->GetFloatValue(RotProgress) : RotProgress;
	NewRot.Yaw = FMath::Lerp(RotStart, RotStop, Alpha);
	SetActorRotation(NewRot);
	
	if (RotProgress>1) SetIsRotating(false);
}

void ALDoor::SetText_Implementation() {
	Interact->Text = Texts[IsOpen?0:1];
}

void ALDoor::SetIsRotating(bool NewIsRotating) {
	IsRotating = NewIsRotating;
	SetActorTickEnabled(IsRotating);
	if (!IsRotating) return;

	const float RotTarget = Rots[IsOpen?0:1];
	RotStart = GetActorRotation().Yaw;
	RotStop = RotBegin.Yaw + RotTarget;
	RotProgress = 0.0;
}

void ALDoor::Trigger_Implementation() {
	if (IsRotating) return;
	IsOpen = !IsOpen;
	
	SetText();
	SetIsRotating(true);
	UE_LOG(LogTemp, Log, TEXT("Door changed open=%i"), IsOpen ? 0:1);
}
