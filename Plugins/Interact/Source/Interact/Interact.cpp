// Copyright Jerónimo Barraco-Mármol

#include "Interact.h"

#include "CInteract.h"

AInteract::AInteract():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false); // ensure we don't animate on start
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	IRoot = CreateDefaultSubobject<USceneComponent>(TEXT("IRoot"));
	IRoot->SetupAttachment(RootComponent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(IRoot);
	Mesh->PrimaryComponentTick.bStartWithTickEnabled = false;
	Mesh->SetComponentTickEnabled(false);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh->SetCanEverAffectNavigation(false);

	Interact = CreateDefaultSubobject<UCInteract>(TEXT("Interact"));
	Interact->SetupAttachment(Mesh);
	Interact->HoverMesh = Mesh;
	// added so that it gets reparented. but ideally it should happen on the component.
	// Interact->PostProcess->AttachToComponent(IRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

bool AInteract::TryTrigger_Implementation() {
	if (Locked) {
		TriggerLocked();
		return false;
	}

	Trigger();
	return true;
}

EItemUseResult AInteract::TryUseItem_Implementation(const FName& Name) {
	UE_LOG(LogTemp, Log, TEXT("Ainteract.TryUseItem=%s"), *Name.ToString());
	return EItemUseResult::BAD_TARGET;
}

void AInteract::SetEnabled(bool Enabled) {
	if (IsValid(Interact)) {
		Interact->SetEnabled(Enabled);
	}
}

void AInteract::BeginPlay() {
	Super::BeginPlay();
	SetText();

	Interact->OnTrigger.AddUniqueDynamic(this, &AInteract::TryTriggerWrapped);
	Interact->OnHover.AddUniqueDynamic(this, &AInteract::Hover);
}

void AInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Interact->OnTrigger.RemoveAll(this);
	Interact->OnHover.RemoveAll(this);
	Interact->DeInit();
}

void AInteract::SetText_Implementation() {}

void AInteract::SetInteractAutoBounds() {
	// Mesh->GetLocalBounds();
	// TODO how do i get the mesh size?
	Interact->Bounds = Mesh->GetPlacementExtent();
}

void AInteract::Trigger_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Actor Triggered"));
	SetText();
}

void AInteract::Hover_Implementation(bool IsOn) {}