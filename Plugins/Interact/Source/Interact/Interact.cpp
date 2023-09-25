// Copyright Jerónimo Barraco-Mármol

#include "Interact.h"

#include "CInteract.h"
#include "Components/AudioComponent.h"
#include "JUtils/Actors/CQuickMesh.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteract, Log, Log);

AInteract::AInteract():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	// ensure we don't animate on start
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.bStartWithTickEnabled = false;

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
	UE_LOG(LogInteract, Log, TEXT("TryUseItem=%s"), *Name.ToString());
	return EItemUseResult::BAD_TARGET;
}

void AInteract::SetEnabled(bool Enabled) {
	if (IsValid(Interact)) {
		Interact->SetEnabled(Enabled);
	}
}

void AInteract::SetMobility(EComponentMobility::Type Mobility) {
	Mesh->SetMobility(Mobility);
	Interact->SetMobility(Mobility);
	RootComponent->SetMobility(Mobility);
	IRoot->SetMobility(Mobility);
	SFX->SetMobility(Mobility);
}

void AInteract::BeginPlay() {
	Super::BeginPlay();
	SetText();

	Interact->OnTrigger.AddUniqueDynamic(this, &AInteract::TryTriggerWrapped);
	Interact->OnHover.AddUniqueDynamic(this, &AInteract::Hover);
}

void AInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(Interact)) {
		Interact->OnTrigger.RemoveAll(this);
		Interact->OnHover.RemoveAll(this);
		Interact->DeInit();
	}
	Interact = nullptr;
	Super::EndPlay(EndPlayReason);
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
	UE_LOG(LogInteract, Log, TEXT("Actor Triggered"));
	SetText();
	PlaySFX(SFX_Trigger);
}

void AInteract::PlaySFX(USoundBase* Snd) {
	if (!IsValid(Snd)) return;

	UE_LOG(LogInteract, Log, TEXT("Playing sound %s "), *Snd->GetName());
	SFX->SetHiddenInGame(false);
	SFX->SetSound(Snd);
	SFX->SetActive(true, true);
	SFX->Play(0);
}

void AInteract::Hover_Implementation(bool IsOn) {}
