// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "Interact.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "JUtils/Actors/CQuickMesh.h"
#include "JUtils/Net/JUtilsNet.h"

#include "CInteract.h"
#include "CInteractor.h"

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
	// Interact->PhysComp = Cast<UPrimitiveComponent>(Mesh);

	// i thought on making this a CSounder. but i don't really need it.
	// and it's on a different plugin package, and i don't want to depend on it.
	SFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX"));
	SFX->SetupAttachment(Interact);
	SFX->SetAutoActivate(false);
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

void AInteract::Grab(const bool IsGrab, UCInteractor* const NewParent) {
	return;
}

EItemUseResult AInteract::TryUseItem_Implementation(const FName& Name) {
	UE_LOG(LogInteract, Log, TEXT("%hs Item=%s Obj=%s"), __func__,
		*Name.ToString(), *GetNameSafe(this));
	return EItemUseResult::BAD_TARGET;
}

void AInteract::SetActive_Implementation(const bool Active) {
	UE_LOG(LogInteract, Log, TEXT("%hs Enabled=%i Obj=%s"),
		__func__, Active, *GetNameSafe(this));

	if (!IsValid(Interact)) {
		UE_LOG(LogInteract, Warning, TEXT("%hs Interact is invalid!!!!!!!"), __func__);
		return;
	}

	Interact->SetActive(Active);
}

void AInteract::SetAutoActivate(const bool AutoActive) {
	UE_LOG(LogInteract, Log, TEXT("%hs: AutoActive=%i Server=%i Obj=%s"),
		__func__, AutoActive, JU_IsServerSide, *GetNameSafe(this));
	if (Interact) Interact->SetAutoActivate(AutoActive);
}

bool AInteract::GetEnabled() const {
	const bool Enabled = IsValid(Interact) && Interact->IsActive();
	UE_LOG(LogInteract, Log, TEXT("%hs: Enabled=%i Server=%i Obj=%s"),
		__func__, Enabled, JU_IsServerSide, *GetNameSafe(this));
	return Enabled;
}

void AInteract::SetMobility(const EComponentMobility::Type Mobility) {
	Mesh->SetMobility(Mobility);
	Interact->SetMobility(Mobility);
	RootComponent->SetMobility(Mobility);
	IRoot->SetMobility(Mobility);
	SFX->SetMobility(Mobility);
}

void AInteract::Reset() {
	UE_LOG(LogInteract, Log, TEXT("%hs: Obj=%s"), __func__, *GetNameSafe(this));
	Super::Reset();
	// don't set state here. since it can break some stuff
	// unfortunately i don't remember what.
	// SetState(0);
}

void AInteract::SetState_Implementation(const int32 NewState) {
	UE_LOG(LogInteract, Log, TEXT("%hs: NewState=%i Obj=%s"),
		__func__, NewState, *GetNameSafe(this));
	State = NewState;
	SetText();
}

void AInteract::BeginPlay() {
	Super::BeginPlay();
	SetText();

	Interact->OnTrigger.AddUniqueDynamic(this, &AInteract::TryTriggerWrap);
	Interact->OnHover.AddUniqueDynamic(this, &AInteract::Hover);
	Interact->OnGrab.AddUniqueDynamic(this, &AInteract::Grab);

	if (Mesh->IsSimulatingPhysics())
		Interact->PhysComp = Mesh;
}

void AInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(Interact)) {
		Interact->OnTrigger.RemoveAll(this);
		Interact->OnHover.RemoveAll(this);
		Interact->OnGrab.RemoveAll(this);
		Interact->DeInit();
	}
	Interact = nullptr;

	Super::EndPlay(EndPlayReason);
}

void AInteract::DoTriggerLocked_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));
	PlaySFX(SFX_Locked);
}

void AInteract::SetInteractAutoBounds() {
	// Mesh->GetLocalBounds();
	// TODO how do i get the mesh size?
	// Mesh->GetLocalBounds() params?
	Interact->Bounds = Mesh->GetPlacementExtent();
}

void AInteract::DoTrigger_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs: Server=%i, Role=%s, Obj=%s"),
		__func__, JU_IsServerSide, *UEnum::GetValueAsString(GetLocalRole()),
		*GetNameSafe(this));

	// set the state before, so that the sound triggers are consistent
	const int32 NewState = (State +1) % StateNum;
	SetState(NewState);
	PlaySFX(SFX_Trigger);

	for(const TObjectPtr<AInteract>& I: RewardInterEnable) {
		if (LIKELY(IsValid(I))) I->SetActive(true);
	}
	
	if (IsOneShot) SetActive(false);
}

void AInteract::PlaySFX_Implementation(USoundBase* Snd) const {
	// When replicated this will play on server and all clients (when called by trigger or server)
	// if called by a sim proxy it will be heard only on the sim proxy. which is good for now.
	// we don't want to spam "hover" sounds anyway.
	
	if (!IsValid(Snd)) return;
	UE_LOG(LogInteract, Log, TEXT("%hs: Attached=%i, Server=%i, Role=%s Obj=%s Snd=%s."),
		__func__, UseAttachedSFX, JU_IsServerSide, *UEnum::GetValueAsString(GetLocalRole()),
		*GetNameSafe(this), *Snd->GetName());

	if (UseAttachedSFX) {
		SFX->SetHiddenInGame(false);
		SFX->SetSound(Snd);
		SFX->SetActive(true, true);
		// this is not necessary, but it might avoid issues when playing before the sound ends
		// SFX->Play(0);
		return;
	}

	// little patch to test whether we can play un-attached sounds with a sound class.
	// will this change ALL instances loaded of this sound? who knows!?
	// do i care? Not now. past-me thinks future-me should not need to play the same sound on two different classes.
	if (SFX->SoundClassOverride) Snd->SoundClassObject = SFX->SoundClassOverride;

	UGameplayStatics::PlaySoundAtLocation(this, Snd,
		SFX->GetComponentLocation(), SFX->GetComponentRotation());
		// ,1, 1, 0, SFX->AttenuationSettings
		// // I could add the attenuation but since it's not attached with the object it occludes with it!
}
