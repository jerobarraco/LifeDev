// Copyright Jerónimo Barraco-Mármol

#include "Interact.h"

#include "JUtils/CQuickMesh.h"

#include "CInteract.h"
#include "Components/AudioComponent.h"

AInteract::AInteract():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false); // ensure we don't animate on start

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	IRoot = CreateDefaultSubobject<USceneComponent>(TEXT("IRoot"));
	IRoot->SetupAttachment(RootComponent);
	
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(IRoot);

	Interact = CreateDefaultSubobject<UCInteract>(TEXT("Interact"));
	Interact->SetupAttachment(Mesh);
	Interact->HoverMesh = Mesh;
	// added so that it gets reparented. but ideally it should happen on the component.
	// Interact->PostProcess->AttachToComponent(IRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	SFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX"));
	SFX->SetupAttachment(IRoot);
	SFX->SetAutoActivate(false);
	SFX->SetHiddenInGame(true);
	SFX->bAutoManageAttachment = true;
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

void AInteract::TriggerLocked_Implementation() {
	PlaySFX(SFX_Locked);
}

void AInteract::SetInteractAutoBounds() {
	// Mesh->GetLocalBounds();
	// TODO how do i get the mesh size?
	// Mesh->GetLocalBounds() params?
	Interact->Bounds = Mesh->GetPlacementExtent();
}

void AInteract::Trigger_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Actor Triggered"));
	SetText();
	PlaySFX(SFX_Trigger);
}

void AInteract::PlaySFX(USoundBase* Snd) {
	if (!IsValid(Snd)) return;
	SFX->SetHiddenInGame(false);
	SFX->SetSound(Snd);
	SFX->SetActive(true, true);
	SFX->Play(0);
}

void AInteract::Hover_Implementation(bool IsOn) {}