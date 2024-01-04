// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "Interact.h"

#include "CInteract.h"
#include "Components/AudioComponent.h"
#include "JUtils/Actors/CQuickMesh.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteract, Log, Log);

AInteract::AInteract():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	// ensure we don't animate on start
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	IRoot = CreateDefaultSubobject<USceneComponent>(TEXT("IRoot"));
	IRoot->SetupAttachment(RootComponent);
	
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(IRoot);

	Interact = CreateDefaultSubobject<UCInteract>(TEXT("Interact"));
	Interact->SetupAttachment(Mesh);
	Interact->HoverMesh = Mesh;

	SFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX"));
	SFX->SetupAttachment(Mesh);
	SFX->SetAutoActivate(false);
	SFX->SetHiddenInGame(true);
	SFX->bAutoManageAttachment = true;
}

bool AInteract::TryTrigger_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs Obj=%s"), __func__, *GetNameSafe(this));
	if (Locked) {
		TriggerLocked();
		return false;
	}

	Trigger();
	return true;
}

EItemUseResult AInteract::TryUseItem_Implementation(const FName& Name) {
	UE_LOG(LogInteract, Log, TEXT("%hs Obj=%s Item=%s"), __func__, *GetNameSafe(this), *Name.ToString());
	return EItemUseResult::BAD_TARGET;
}

void AInteract::SetEnabled(bool Enabled) {
	if (!IsValid(Interact)) return;

	Interact->SetEnabled(Enabled);
}

void AInteract::SetMobility(EComponentMobility::Type Mobility) {
	Mesh->SetMobility(Mobility);
	Interact->SetMobility(Mobility);
	RootComponent->SetMobility(Mobility);
	IRoot->SetMobility(Mobility);
	SFX->SetMobility(Mobility);
}

void AInteract::Reset() {
	UE_LOG(LogInteract, Log, TEXT("%hs: Obj=%s"), __func__, *GetNameSafe(this));
	Super::Reset();
	// SetState(0);
}

void AInteract::SetState_Implementation(int32 NewState) {
	UE_LOG(LogInteract, Log, TEXT("%hs: NewState=%i Obj=%s"), __func__, NewState, *GetNameSafe(this));
	State = NewState;
	SetText();
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
	UE_LOG(LogInteract, Log, TEXT("%hs : %s"), __func__, *GetNameSafe(this));
	// set the state before, so that the sound triggers are consistent
	const int32 NewState = (State +1) % StateNum;
	SetState(NewState);
	PlaySFX(SFX_Trigger);
}

void AInteract::PlaySFX(USoundBase* Snd) {
	if (!IsValid(Snd)) return;
	UE_LOG(LogInteract, Log, TEXT("%hs: %s: Playing sound. attached=%i, name='%s'."),
		__func__, *GetNameSafe(this), UseAttachedSFX, *Snd->GetName());

	if (UseAttachedSFX) {
		SFX->SetHiddenInGame(false);
		SFX->SetSound(Snd);
		SFX->SetActive(true, true);
		// this is not necessary, but it might avoid issues when playing before the sound ends
		// SFX->Play(0);
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(SFX, Snd,
		SFX->GetComponentLocation(), SFX->GetComponentRotation());
}
