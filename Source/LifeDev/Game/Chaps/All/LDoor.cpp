// Copyright Jerónimo Barraco-Mármol

#include "LDoor.h"

#include "Interact/CInteract.h"


ALDoor::ALDoor(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	Interact->SetRelativeTransform(FTransform(
		FRotator::ZeroRotator,
		FVector(25,80,125),
		FVector::OneVector
	));
}

void ALDoor::BeginPlay() {
	Super::BeginPlay();
	RotClosed = GetActorRotation();
	RotOpen = RotClosed + Rot;
}

void ALDoor::Tick(float DT) {
	Super::Tick(DT);
	if (!IsRotating) return;

	// adjust for duration
	const float ndt = DT/RotDuration;
	RotProgress += ndt;

	// not using a lerp because lerping with rotations has the nice properties that -90 becomes 270 and spins the other way around
	const float Alpha = IsValid(RotCurve)? RotCurve->GetFloatValue(RotProgress) : RotProgress;
	FRotator NewRot = RotStart + (RotDelta*Alpha);
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

	RotStart = IsOpen ? RotOpen : RotClosed;
	RotDelta = IsOpen ? Rot * -1 : Rot;
	RotProgress = 0.0;
}

void ALDoor::Trigger_Implementation() {
	if (IsRotating) return;
	
	SetText();
	SetIsRotating(true);
	// change the flag after we start rotating. now the change has happened.
	IsOpen = !IsOpen;
	UE_LOG(LogTemp, Log, TEXT("Door changed open=%i"), IsOpen ? 0:1);
}
