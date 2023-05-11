// Copyright Jerónimo Barraco-Mármol

#include "LDoor.h"

#include "LifeDev/Core/Interacts/CInteract.h"

ALDoor::ALDoor(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false); // ensure we don't animate on start

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetComponentTickEnabled(false);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh->SetCanEverAffectNavigation(false);

	Interact = CreateDefaultSubobject<UCInteract>(TEXT("Interact"));
	Interact->SetupAttachment(Mesh);
	Interact->SetComponentTickEnabled(false);
	Interact->SetRelativeTransform(FTransform(
		FRotator::ZeroRotator,
		FVector(25,80,125),
		FVector::OneVector
	));
}

void ALDoor::BeginPlay() {
	Super::BeginPlay();
	RotBegin = GetActorRotation();
	SetText();
	Interact->OnTrigger.AddUniqueDynamic(this, &ALDoor::Trigger);
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

void ALDoor::SetText() {
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

void ALDoor::Trigger() {
	if (IsRotating) return;
	IsOpen = !IsOpen;
	
	SetText();
	SetIsRotating(true);
	UE_LOG(LogTemp, Log, TEXT("Door changed open=%i"), IsOpen ? 0:1);
}
