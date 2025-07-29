// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "Interact.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "JUtils/Actors/CQuickMesh.h"
#include "Animator/Anim.h"

#include "CInteract.h"
#include "Eval.h"

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
	// parented to the mesh to make it easier to move stuff around when the mesh changes.
	// or when i subclass and put another mesh, or things like that.
	Interact->SetupAttachment(Mesh);
	Interact->HoverMesh = Mesh;
	// Interact->PhysComp = Cast<UPrimitiveComponent>(Mesh);

	// i thought on making this a CSounder. but i don't really need it.
	// and it's on a different plugin package, and i don't want to depend on it.
	SFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX"));
	SFX->SetupAttachment(Interact);
	SFX->SetAutoActivate(false);
	SFX->bAutoManageAttachment = true;

	Emitter = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Emitter"));
	Emitter->SetupAttachment(Interact);
	Emitter->SetAutoActivate(false);
	Emitter->SetUseAutoManageAttachment(true);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/PulseOut.PulseOut"));
	HintCurve = CCurve.Object;
}

bool AInteract::TryTrigger_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs Obj=%s"), __func__, *Label.ToString());
	
	if (UNLIKELY(IsLocked && ShouldUnlock()))
		Unlock();

	if (IsLocked) {
		TriggerLocked();
		return false;
	}

	Trigger();
	return true;
}

void AInteract::Grab(const bool IsGrab, UCInteractor* const NewParent) {
	return;
}

void AInteract::SetText_Implementation() {
	const int32 Num = Texts.Num();
	if (UNLIKELY(Num < 1)) {
		UE_LOG(LogInteract, Warning, TEXT("AInteractAnim.SetText: Object has no text to set"));
		return;
	}

	if (UNLIKELY((State < 0) | (State >= Num))) {
		UE_LOG(LogInteract, Log, TEXT("AInteractAnim.%hs: Can't set text with invalid state=%i textnum=%i"),
			__func__, State, Num);
		return;
	}

	Interact->Text = Texts[State];
	UE_LOG(LogInteract, Log, TEXT("AInteractAnim.SetText: State=%i, NewText=%s"), State, *Interact->Text.ToString());
}

void AInteract::SetActive_Implementation(const bool Active) {
	UE_LOG(LogInteract, Log, TEXT("%hs: Enabled=%i Obj=%s"),
		__func__, Active, *Label.ToString());

	if (LIKELY(IsValid(Interact))) Interact->SetActive(Active);
}

bool AInteract::GetActive() const {
	return LIKELY(IsValid(Interact)) && Interact->IsActive();
}

void AInteract::SetMobility(const EComponentMobility::Type Mobility) {
	Mesh->SetMobility(Mobility);
	Interact->SetMobility(Mobility);
	RootComponent->SetMobility(Mobility);
	IRoot->SetMobility(Mobility);
	SFX->SetMobility(Mobility);
}

void AInteract::Reset() {
	UE_LOG(LogInteract, Log, TEXT("%hs: Obj=%s"), __func__, *Label.ToString());
	Super::Reset();
	// don't set state here. since it can break some stuff
	// unfortunately i don't remember what.
	// SetState(0);
}

void AInteract::SetStateNow_Implementation(const int32 NewState, const bool UseSFX, const bool UseParts) {
	UE_LOG(LogInteract, Log, TEXT("%hs: NewState=%i Obj=%s"),
		__func__, NewState, *Label.ToString());
	// this duplicates SetState. make sure to modify that one too.
	if (UNLIKELY((State <0) | (NewState >= StateNum))) return;

	State = NewState;
	SetText();

	// notice state <0 is checked above
	if ((UseSFX & UseAttachedSFX & bool(SFX) & (State < SFXs.Num())) && SFXs[State]) {
		SFX->SetSound(SFXs[State]);
		SFX->SetAutoActivate(true);
	}

	if ((UseParts & bool(Emitter) & (State < Particles.Num())) && Particles[State]) {
		Emitter->SetAsset(Particles[State]);
		Emitter->SetAutoActivate(true);
	}
}

void AInteract::SetState_Implementation(const int32 NewState) {
	UE_LOG(LogInteract, Log, TEXT("%hs: NewState=%i Obj=%s"),
		__func__, NewState, *Label.ToString());
	// this duplicates SetStateNow. make sure to modify that one too.

	if (UNLIKELY((State <0) | (NewState >= StateNum))) return;

	State = NewState;
	SetText();
	if ((State >=0) & (State < SFXs.Num()))
		PlaySFX(SFXs[State]);
	if ((State >=0) & (State < Particles.Num()))
		PlayParts(Particles[State]);
}

bool AInteract::ShouldUnlock_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs Obj=%s"), __func__, *Label.ToString());

	if (UnlockCondition.IsEmpty()) return false; // only thing to check here.

	const UEval* const Eval = UEval::Instance(this);
	double Res = -1;
	if (LIKELY(Eval)) Eval->Eval(UnlockCondition, Res);
	
	const bool Passed = Res > 0;
	UE_LOG(LogInteract, Log,
		TEXT("%hs Attempt to unlock with condition='%s', Res=%.4f, Pass=%i"),
		__func__, *UnlockCondition, Res, Passed);

	return Passed;
}

void AInteract::Unlock_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs Obj=%s"), __func__, *Label.ToString());
	if (UNLIKELY(!IsLocked)) return; // avoid re-triggering stuff
	IsLocked = false; // force unlock
}

bool AInteract::ShowHint_Implementation() {
	UE_LOG(LogInteract, Verbose, TEXT("%hs Obj=%s UseHint=%i Hidden=%i IsActive=%i PrimId=%i"), __func__, *Label.ToString(),
		UseHint, IsHidden(), Interact->IsActive(), HintPrimDataID);
	if (!UseHint | IsHidden() | !Interact->IsActive()) return false;

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return false;

	if (!HintCondition.IsEmpty()) {
		const UEval* const Eval = UEval::Instance(this);
		if (UNLIKELY(!Eval)) return false; // nopes
		
		double Res;
		if (UNLIKELY(!Eval->Eval(HintCondition, Res))) return false; // nopes
		if (Res <= 0) return false; // nopess
	}

	Interact->Hint(true);
	PlaySFX(SFXHint); // sfx checked inside

	// schedule unhint
	FTimerHandle H;
	auto F = [this]() {
		if (UNLIKELY(!IsValid(this) | !IsValid(Interact))) return;
		Interact->Hint(false);
	};
	World->GetTimerManager().SetTimer(H, F, HintTime, false, -1);

	OnHint.Broadcast(); // dispatch here (avoid return. also the rest is just animation)

	// also animate a custom primitive data.
	UAnim* const AnimMat = UAnim::Instance(this);
	if (UNLIKELY(!AnimMat | (HintPrimDataID<0))) return true; // shown, so need to return true anyway

	FAParams P;
	P.Duration = 0;
	// ensure from 0
	AnimMat->DataFade(Mesh, P, HintPrimDataID, true, FLinearColor::Black);
	// the curve helps with the animation. it could be done on the material as well. doushio?
	// i think this makes it clearer for art people.
	P.Duration = HintTime;
	P.Curve = HintCurve.Get();
	AnimMat->DataFade(Mesh, P, HintPrimDataID, true, FLinearColor::White, false);

	return true;
}

void AInteract::BeginPlay() {
	Super::BeginPlay();
	SetText();

	if (Mesh->IsSimulatingPhysics())
		Interact->PhysComp = Mesh;

	// now activating if it needs to.
	const bool IsActive = Interact->IsActive();
	// deactivate if it's hidden. (fixes some things)
	// it will activate back on SetActorHiddenInGame(true) if AutoActivate is true
	if (UNLIKELY(IsHidden())) SetActive(false);
	else if (UseAutoActivate != IsActive)
		SetActive(UseAutoActivate); // this will also disable if it's active. so whoever uses this class will have to be careful.

	Interact->OnTrigger.AddUniqueDynamic(this, &AInteract::TryTriggerWrap);
	Interact->OnHover.AddUniqueDynamic(this, &AInteract::Hover);
	Interact->OnGrab.AddUniqueDynamic(this, &AInteract::Grab);

	HintCurve.LoadSynchronous(); // ensure it's loaded.
}

void AInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(IsValid(Interact))) {
		Interact->OnTrigger.RemoveAll(this);
		Interact->OnHover.RemoveAll(this);
		Interact->OnGrab.RemoveAll(this);
		Interact->DeInit();
	}

	Interact = nullptr;
	RewardIntersActive.Empty(0);
	Super::EndPlay(EndPlayReason);
}

void AInteract::EditorLabelUpd(AActor* const Actor) { // can't be const. it's a binding.
	// let the compiler remove this function on runtime. because compilers are smart.
#if WITH_EDITORONLY_DATA
	if (LIKELY(Actor != this)) return;
	// always rewrite (not checking empty)
	Label = FName(GetActorLabel(true));
#endif
}

void AInteract::InitLabel() {
	UE_LOG(LogInteract, Log, TEXT("%hs Label=%s Name=%s"), __func__,
		*Label.ToString(), *GetNameSafe(this));
#if WITH_EDITORONLY_DATA
	if (UNLIKELY(Label.IsNone())) // allow to be overriden. otherwise load from serialized
		EditorLabelUpd(this);
	// automatic update
	FCoreDelegates::OnActorLabelChanged.AddUObject(this, &AInteract::EditorLabelUpd);
#else
	// label shouldn't be none since it should have been serialized. we're in trouble. or it's a dynamically spawned object and i don't care then.
	UE_CLOG(UNLIKELY(Label.IsNone()), LogInteract, Warning, TEXT("%hs Label is none! Label=%s Name=%s"), __func__,
		*Label.ToString(), *GetNameSafe(this));
	if (UNLIKELY(Label.IsNone())) // allow to be overriden. otherwise load from serialized
		Label = GetFNameSafe(this);
#endif
}

void AInteract::PostLoad() {
	// called when the class is loaded. called on runtime.
	Super::PostLoad();
	UE_LOG(LogInteract, Log, TEXT("%hs l=%s n=%s AutoActivate=%i"),
		__func__, *Label.ToString(), *GetNameSafe(this), UseAutoActivate);
	// this function is only called on objects on the level so ActorLabel should be correct

	InitLabel();
}

void AInteract::PostActorCreated() {
	Super::PostActorCreated();
	UE_LOG(LogInteract, Log, TEXT("%hs l=%s n=%s"), __func__, *Label.ToString(), *GetNameSafe(this));
	// Only called on spawning actors AND DUPLICATIONS
	// this function is mutually exclusive with PostLoad according to the docs
	InitLabel();
}

void AInteract::SetActorHiddenInGame(const bool NewHidden) {
	Super::SetActorHiddenInGame(NewHidden);
	if (NewHidden | UseAutoActivate) SetActive(!NewHidden);
}

void AInteract::DoTriggerLocked_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs l=%s"), __func__, *Label.ToString());
	PlaySFX(SFXLocked);
}

void AInteract::SetInteractAutoBounds() {
	// Mesh->GetLocalBounds();
	// TODO how do i get the mesh size?
	// Mesh->GetLocalBounds() params?
	Interact->Bounds = Mesh->GetPlacementExtent();
}

void AInteract::DoTrigger_Implementation() {
	UE_LOG(LogInteract, Log, TEXT("%hs: Obj=%s"),
		__func__, *Label.ToString());

	if (LIKELY(UseTriggerDeHint)) UseHint = false; // clear hint flag.

	// set the state before, so that the sound triggers are consistent
	if (LIKELY(StateNum > 0)) { // mostly a fix.
		// If StateNum ==1 NewState will be == State. i do not care. trigger SetState anyway, it's on purpose.
		int32 NewState = State;
		++NewState;
		if (UNLIKELY(NewState >= StateNum))
			NewState = UseStateLoop ? NewState % StateNum : StateNum-1;
		SetState(NewState);
	}

	for(const TSoftObjectPtr<AInteract>& SI: RewardIntersActive) {
		AInteract* const I = SI.Get();
		if (LIKELY(IsValid(I)))
			I->SetActive(true);
		else
			UE_LOG(LogInteract, Warning, TEXT("%hs Actor is not valid."
				" Potentially not loaded. O=%s"),
				__func__, *SI->GetPathName());
	}

	for(const TSoftObjectPtr<AInteract>& SI: RewardIntersHint) {
		AInteract* const I = SI.Get();
		// important to use this to respect children (e.g. puzzles).
		// this is the whole reason i've implemented this virtual method.
		if (LIKELY(IsValid(I)))
			I->SetUseHint(true);
		else
			UE_LOG(LogInteract, Warning, TEXT("%hs Actor is not valid."
				" Potentially not loaded. O=%s"),
				__func__, *SI->GetPathName());
	}

	for(const TSoftObjectPtr<AInteract>& SI: RewardIntersTrigger) {
		AInteract* const I = SI.Get();
		if (LIKELY(IsValid(I)))
			I->TryTrigger();
		else
			UE_LOG(LogInteract, Warning, TEXT("%hs Actor is not valid."
				" Potentially not loaded. O=%s"),
				__func__, *SI->GetPathName());
	}

	if (IsOneShot) SetActive(false);
}

void AInteract::PlaySFX(USoundBase* const Snd) const {
	UE_LOG(LogInteract, Log, TEXT("%hs: Attached=%i Obj=%s Snd=%s"),
		__func__, UseAttachedSFX, *Label.ToString(), *GetNameSafe(Snd));
	// very important because it's triggered from multiple places.
	// and some places need to have a nullptr for space (like SFXs).
	if (UNLIKELY(!IsValid(Snd))) {
		// stop the sfx if it's playing. i could have used a CSounder, but i don't really need it
		// and i don't want to break my game right now.
		// this is used for looping sounds
		if (UNLIKELY(SFX->IsPlaying())) SFX->FadeOut(1, 0);
		return;
	}

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

void AInteract::PlayParts(UNiagaraSystem* const Part) const {
	UE_LOG(LogInteract, Log, TEXT("%hs: Obj=%s Part=%s"),
		__func__, *Label.ToString(), *GetNameSafe(Part));

	if (LIKELY(Emitter->GetAsset() != Part)) { // opt
		UE_LOG(LogInteract, Log, TEXT("%hs: Deactivate old one"), __func__);
			Emitter->Deactivate();
		if (Part) { // let the system stop by itself
			Emitter->ResetSystem();
			Emitter->SetAsset(Part);
		}
	} else
		UE_LOG(LogInteract, Log, TEXT("%hs: Reactivating old one"), __func__);

	if (UNLIKELY(!IsValid(Part))) return; // check here to allow to stop the current particles

	// done like this to allow to re-activate emitters with limited lifetime. (non-loop)
	Emitter->Activate();
}

/*
 un-necessary. interact is not active by default. so UseAutoActivate will control the activation always.
void AInteract::SetAutoActivate(const bool AutoActive) {
	UE_LOG(LogInteract, Log, TEXT("%hs: AutoActive=%i Obj=%s Inter=%p"),
		__func__, AutoActive, *Label.ToString(), Interact.Get());
	if (LIKELY(Interact)) {
		Interact->bAutoActivate = AutoActive;
		Interact->SetAutoActivate(AutoActive);
	}

	// this is to ensure the state is consistent.
	// the var has priority since it has to be able to be set on editor.
	UseAutoActivate = AutoActive;
}*/