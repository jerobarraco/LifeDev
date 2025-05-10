// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Camera/CameraComponent.h" // needed even though rider disagrees

#include "Diags/Diags.h"
#include "Interact/Animator/CAnimatorTrans.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "Story/Story.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/LInteract.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLStoryStep, Log, Log);

ALStep::ALStep():Super() {
	RandFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RandFB"));

	// classes doesn't work with the ".CamShake_B" ending. (i think they need the _B_C at end) 
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>
		CShake(TEXT("/Game/LifeDev/Game/Env/CamShake_B"));
	CamShakeClass = CShake.Class;
	// disappeared from ue5.4 without warning
	// UDefaultCameraShakeBase::StaticClass();

	Anim = CreateDefaultSubobject<UCAnimatorTrans>(TEXT("Anim"));
	Anim->TRoot = Cam;

	AnimTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));
	AnimTarget->SetupAttachment(Root);

#if WITH_EDITORONLY_DATA
	AnimCam = CreateDefaultSubobject<UCameraComponent>(TEXT("AnimCam"));
	AnimCam->SetupAttachment(AnimTarget);
	AnimCam->SetComponentTickEnabled(false);
	AnimCam->SetVisibility(false);
	AnimCam->SetHiddenInGame(true); // for preview only
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCam (TEXT("/Niagara/DefaultAssets/S_Gnomon"));
	if (CCam.Succeeded()) AnimCam->SetCameraMesh(CCam.Object);
#endif
}

void ALStep::TryStart_Implementation() {
	// initialize cam and anim for an appropriate cam blend
	if (UseFBAnim & LIKELY(bool(Anim) & bool(AnimTarget))) {
		// the animator trans uses relative transforms always :/
		Anim->CopyTStart();
		Anim->TEnd = AnimTarget->GetRelativeTransform();
	}

	Super::TryStart_Implementation();

	EnsureItems(); // make sure items are awarded

	// disable the input during camblend
	// works on the premise that onStart it will force input again.
	if (!CamTarget) return;
	const UWorld* const W = GetWorld();
	AGameModeBase* const GameModeBase = LIKELY(W) ? W->GetAuthGameMode() : nullptr;
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	if (LIKELY(IsValid(LGGameMode))) LGGameMode->SetCharInputEnabled(false);
}

void ALStep::Stop_Implementation() {
	if (LIKELY(IsValid(Diags))) {
		Diags->OnDone.RemoveAll(this);
		Diags->OnAdd.RemoveAll(this);
		Diags->OnShow.RemoveAll(this);
	}
	if (LIKELY(IsValid(Inventory))) Inventory->OnMod.RemoveAll(this);
	if (LIKELY(IsValid(FB))) FB->OnChange.RemoveAll(this);
	if (LIKELY(IsValid(Ghosts))) Ghosts->SetPlaying(false);
	if (LIKELY(IsValid(RandFB))) RandFB->Deactivate();
	if (LIKELY(IsValid(Flags))) Flags->OnMod.RemoveAll(this);

	SetActorsShowActive(false, true);
	SetIntersActiveAuto(false);
	RemoveItems();

	const UWorld* const W = GetWorld();
	if (UseRain) ALMusicMan::SetRainS(W, false);
	if (LIKELY(W)) { // call stop anyway (below)
		FTimerManager& Timer = W->GetTimerManager();
		// ensure we don't double trigger.
		// this timer is stored in the class since clearAllTimers here could accidentally stop timers from child classes.
		// anyway timers on or after Stop are really dangerous as the class could be unloaded.
		// Proof of that is the patch i had to do with destroy actors.
		Timer.ClearTimer(TimerDestroy);
		// Destroy them during the fade
		TimerDestroy.Invalidate();
		Timer.SetTimer(TimerDestroy, this, &ALStep::DestroyActors, 2);
	}

	Super::Stop_Implementation(); // do at end.
}

void ALStep::Start_Implementation() {
	Super::Start_Implementation();
	UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	// check items. do on Start to avoid possibly finishing the step while it's starting.
	if (!ItemsFinish.IsEmpty()) {
		Inventory->OnMod.AddUniqueDynamic(this, &ALStep::ItemMod);
	}

	// check flags. do on Start to avoid possibly finishing the step while it's starting.
	if (!FlagsFinish.IsEmpty()) {
		Flags->OnMod.AddUniqueDynamic(this, &ALStep::FlagMod);
	}

	// ensure to check if we already have the item. but not now to not affect the flow of child classes
	W->GetTimerManager().SetTimerForNextTick(this, &ALStep::CheckFinish);

	AGameModeBase* const GameModeBase = W->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get(); // doesn't work
	
	// do only on postwait. otherwise the input is reset before it faded out.
	// read note on TryStart. important to force.
	if (LIKELY(IsValid(LGGameMode))) LGGameMode->SetCharInputEnabled(InputEnabled);

	if (UseGhosts) {
		Ghosts = Cast<AGhosts>(W->SpawnActor(AGhosts::StaticClass()));
		if (LIKELY(IsValid(Ghosts))) {
			Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Ghosts->SetActorRelativeLocation(GhostPos);
			Ghosts->SetPlaying(true);
		}
	}

	if (UseRain) ALMusicMan::SetRainS(W, true);
	if (UseFBRand & LIKELY(IsValid(RandFB))) RandFB->Activate(true);
	
	SetActorsShowActive(true, true);
	SetIntersActiveAuto(true);
	DoIntersFade(IntersFadeIn, true);
	DoIntersFade(IntersFadeOut, false);
	DoIntersDeactive();
	DoIntersActive(); // activate after deactivate. for precedence.
	DoIntersHint(); // hint after activate.
	DoIntersTrigger(); // trigger after activate. and hint.

	if (LIKELY(FB)) {
		// it's ok to set it here. since fbupd is only bound here
		FBFrom = FB->GetVal(); // used for the animfb
		FB->OnChange.AddUniqueDynamic(this, &ALStep::FBUpd);
	}

	// show dialogs
	StartDialogs();
}

void ALStep::StartDialogs() {
	// risky. but bind anyway. so that child classes can still benefit from the callback.
	// and also can trigger their own dialogs and fbdlgauto would still work.
	Diags->OnShow.AddUniqueDynamic(this, &ALStep::DlgShow);

	if (DlgId.IsNone()) return;

	if (LIKELY(UseFBDlgAuto)) SetFBDlgAuto(DlgId);

	Diags->AddId(DlgId);

	FinishAfterDlgs();
}

void ALStep::SetFBDlgAuto(const FName& Id) {
	FDiagGroup Seq;
	int32 Len = 0;
	const bool Ok = Diags->GetGroup(Id, Seq);
	if (Ok) {
		Len = Seq.DiagRows.Num();
	} else {
		FDiag D;
		const bool Ok2 = Diags->GetDiag(Id, D);
		if (LIKELY(Ok2)) Len = 1;
	}

	if (UNLIKELY(Len <= 0)) { // avoid division by 0, but also makes no sense otherwise.
		FBDlgMod = 0; // ensure a non-destructive value is set. though it could break something, it's a bit cleaner.
		return;
	}

	// GetValTo is the correct as opposed to GetVal.
	// Since we want to move the target, the current is going to follow.
	const float FBCurrent = FB->GetValTo();
	FBDlgMod = (FBDlgAutoTo - FBCurrent) / static_cast<float>(Len); // float so that it reaches the end.

	UE_LOG(LogLStoryStep, Log, TEXT("%hs Id=%s DiagMod=%f, DiagAutoTo=%f, Current=%f, Len=%i"),
		__func__, *Id.ToString(), FBDlgMod, FBDlgAutoTo, FBCurrent, Len);
}

void ALStep::FinishAfterDlgs() {
	if (!Diags->GetIsShowing()) {
		Finish();
		return;
	}

	// important that we use Unique here, since it can be added via the flags or items.
	Diags->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
}

void ALStep::RemoveItems() {
	for (const FName& N: ItemsRem) {
		Inventory->Rem(N);
	}
}

void ALStep::EnsureItems() {
	for(const FName& N: ItemsEnsure) {
		Inventory->Ensure(N);
	}
}

void ALStep::DestroyActors() {
	// this function gets called multiple times. beware.
	UE_LOG(LogLStoryStep, Log, TEXT("%hs Name=%s"), __func__, *Name.ToString());

	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	W->GetTimerManager().ClearTimer(TimerDestroy);

	// this doesn't work, since it might destroy the actor while it's still fading.
	// this is a bit dangerous, we can't go back without reloading (which is a stupidly risky feature anyway).
	// also we are not the creators of such actors, so it kinda doesn't make sense
	// for (AActor* const A: ActorsShow) {
		// if (LIKELY(IsValid(A))) A->Destroy();
	// }
	ActorsShow.Empty(); // release the ref

	if (IsValid(Ghosts)) Ghosts->Destroy();
	Ghosts = nullptr;
}

void ALStep::ItemMod_Implementation(const FName& ItemName, int32 Diff, const FItem& Item) {
	CheckFinish(); // this is inefficient. it's checking both items and flags.
}

void ALStep::FlagMod_Implementation(const FName& FlagName, const float Diff, const float Total) {
	CheckFinish(); // this is inefficient. it's checking both items and flags.
}

void ALStep::FBUpd_Implementation(const float Value) {
	if (!UseFBAnim) return;

	// recalculate the range, so that the animation always starts and ends where it should.
	// and the transforms positions are always anchored at the start and end.
	// regardless of the start and end fb value. even if it's increasing or decreasing.
	const float R = (FBDlgAutoTo - FBFrom); // calculate new range
	const float V = (Value - FBFrom);
	if (UNLIKELY(R==0)) return; // division by 0
	const float NV = V/R; // remap

	// notice this depends on the tick interval for the FB.
	// it will give the most accurate animation though.
	// i will go with something simple for now.
	// set the value to match the fb. let the anim do transform blending.
	Anim->Update(NV);
}

void ALStep::CheckFinish() {
	const int32 NumItems = ItemsFinish.Num();
	const int32 NumFlags = FlagsFinish.Num();
	if ((NumItems<1) & (NumFlags<1)) return; // nothing to finish
	if (LIKELY(!HasItemsFinish())) return;
	if (LIKELY(!HasFlagsFinish())) return;
	// clear to avoid any double triggering that might happen while the dialogs are being triggered
	// since flags can change due to many random things, including the timer.
	ItemsFinish.Empty();
	FlagsFinish.Empty();

	const UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimerForNextTick(this, &ALStep::FinishAfterDlgs);
}

bool ALStep::HasItemsFinish() {
	const int32 NumItems = ItemsFinish.Num();
	for (int32 i=0; LIKELY(i<NumItems); ++i) {
		if (!Inventory->Has(ItemsFinish[i])) return false;
	}

	return true;
}

bool ALStep::HasFlagsFinish() {
	const int32 NumFlags = FlagsFinish.Num();
	for (int32 i=0; LIKELY(i<NumFlags); ++i) {
		if (!Flags->Has(FlagsFinish[i])) return false;
	}

	return true;
}

void ALStep::DlgShow_Implementation(const FDiag& Diag) {
	// no need to check for IsNearlyZero. modval does it.
	// ModVal is the best, since if i trigger several dialogs very fast
	// it should still go to the correct value.
	if (LIKELY(FB)) FB->ModVal(FBDlgMod);
}

void ALStep::BeginPlay() {
	Super::BeginPlay();

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	Diags = World->GetSubsystem<UDiags>();
	Inventory = World->GetSubsystem<UInventory>();
	FB = World->GetSubsystem<UFlashback>();
	Flags = World->GetSubsystem<UFlags>();

	SetActorsShowActive(false, false);
	SetIntersActiveAuto(false);
}

void ALStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unbind();
	Diags = nullptr;
	Inventory = nullptr;
	FB = nullptr;
	Flags = nullptr;

	// Ensure we destroy the actors on destroying this actor.
	// could happen if the step is unloaded because the next step unloads the data-layer.
	DestroyActors();

	Super::EndPlay(EndPlayReason); // always at end
}

void ALStep::PostLoad() {
	Super::PostLoad();

	if (UseFadeTime) {
		const UStory* const Story = UStory::Instance(GetWorld());
		WaitTime = LIKELY(Story) ? Story->FadeTime : 1;
	}

	// avoid finishing earlier if we have Diags
	if (!DlgId.IsNone()) FinishPostWait = false;
}

void ALStep::Finish_Implementation() {
	// avoid possible double triggering. since finish is called from several origins.
	Unbind();
	Super::Finish_Implementation();
}

void ALStep::Unbind() const {
	if (LIKELY(IsValid(Diags))) {
		Diags->OnDone.RemoveAll(this);
		Diags->OnShow.RemoveAll(this);
	}
	if (LIKELY(IsValid(Inventory))) Inventory->OnMod.RemoveAll(this);
	if (LIKELY(IsValid(Flags))) Flags->OnMod.RemoveAll(this);
	if (LIKELY(IsValid(FB))) FB->OnChange.RemoveAll(this);
}

void ALStep::SetActorsShowActive(const bool Active, const bool WithFade) {
	for (const TSoftObjectPtr<AActor>& SA: ActorsShow) {
		AActor* const A = SA.Get();
		if (UNLIKELY(!IsValid(A))) continue;

		ALInteract* const Inter = Cast<ALInteract>(A);
		if (Inter && WithFade) { // hide with fade is possible
			Inter->Fade(Active, true); // calls setactorhidden and setactive
			continue;
		}
		// Avoid calling 'Fade' twice, just in case there are side effects.
		A->SetActorHiddenInGame(!Active);
	}
}

void ALStep::DoIntersActiveAny(const TArray<TSoftObjectPtr<AInteract>>& A, const bool NewActive) {
	for (const TSoftObjectPtr<AInteract>& SI : A) {
		AInteract* const I = SI.Get();
		if (LIKELY(IsValid(I))) I->SetActive(NewActive);
	}
}

void ALStep::SetIntersActiveAuto(const bool NewActive) {
	DoIntersActiveAny(IntersActivateAuto, NewActive);
}

void ALStep::DoIntersActive() {
	DoIntersActiveAny(IntersActivate, true);
}

void ALStep::DoIntersDeactive() {
	DoIntersActiveAny(IntersDeactivate, false);
}

void ALStep::DoIntersFade(const TArray<TSoftObjectPtr<ALInteract>>& SA, const bool In) {
	for (const TSoftObjectPtr<ALInteract>& SI: SA) {
		ALInteract* const I = SI.Get();
		if (LIKELY(IsValid(I))) I->Fade(In, true);
	}
}

void ALStep::DoIntersTrigger() const {
	for (const TSoftObjectPtr<ALInteract>& SI: IntersTrigger) {
		ALInteract* const I = SI.Get();
		if (LIKELY(IsValid(I))) I->TriggerForced();
	}
}

void ALStep::DoIntersHint() const {
	for (const TSoftObjectPtr<AInteract>& I: IntersHint) {
		AInteract* const Interact = I.Get();
		if (LIKELY(IsValid(Interact))) Interact->UseHint = true;
	}
}
