// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Diags/Diags.h"
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
}

void ALStep::TryStart_Implementation() {
	Super::TryStart_Implementation();

	EnsureItems(); // make sure items are awarded

	// works on the premise that onStart it will force input again.
	// disable the input during camblend
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
		
		// ensure to check if we already have the item. but not now to not affect the flow of child classes
		W->GetTimerManager().SetTimerForNextTick(this, &ALStep::CheckItemsFinish);
	}
	// check flags. do on Start to avoid possibly finishing the step while it's starting.
	if (!FlagsFinish.IsEmpty()) {
		Flags->OnMod.AddUniqueDynamic(this, &ALStep::FlagMod);
		
		// ensure to check if we already have the item. but not now to not affect the flow of child classes
		W->GetTimerManager().SetTimerForNextTick(this, &ALStep::CheckFlagsFinish);
	}

	AGameModeBase* const GameModeBase = W->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get(); // doesn't work
	
	// do only on postwait. otherwise the input is reset before it faded out.
	// read note on trystart. important to force.
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
	if (UseRandFB && LIKELY(IsValid(RandFB))) RandFB->Activate(true);

	SetActorsShowActive(true, true);
	SetIntersActiveAuto(true);
	DoIntersFade(IntersFadeIn, true);
	DoIntersFade(IntersFadeOut, false);
	DoIntersTrigger();
	DoIntersHint();

	// show dialogs
	StartDialogs();
}

void ALStep::StartDialogs() {
	if (UNLIKELY(DlgId.IsNone())) return;

	SetFBDlgAuto();
	Diags->OnShow.AddUniqueDynamic(this, &ALStep::DlgShow);
	Diags->AddId(DlgId);
	FinishAfterDlgs();
}

void ALStep::SetFBDlgAuto() {
	if (UNLIKELY(!UseFBDlgAuto)) return;

	FDiagGroup Seq;
	int32 Len = 0;
	const bool Ok = Diags->GetGroup(DlgId, Seq);
	if (Ok) {
		Len = Seq.DiagRows.Num();
	} else {
		FDiag D;
		FDiagChar C;
		const bool Ok2 = Diags->GetDiag(DlgId, D, C);
		if (LIKELY(Ok2)) Len = 1;
	}

	// avoid division by 0, but also makes no sense otherwise.
	if (UNLIKELY(Len <= 0)) return;
	// GetValTo is the correct as opposed to GetVal.
	// Since we want to move the target, the current is going to follow.
	const float FBCurrent = FB->GetValTo();
	FBDlgMod = (FBDlgAutoTo - FBCurrent) / Len;

	UE_LOG(LogLStoryStep, Log, TEXT("%hs DiagMod=%f, DiagAutoTo=%f, Current=%f, Len=%i"),
		__func__, FBDlgMod, FBDlgAutoTo, FBCurrent, Len);
}

void ALStep::FinishAfterDlgs() {
	if (!Diags->GetIsShowing()) {
		Finish();
		return;
	}

	// important that we use Unique here
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
	CheckItemsFinish();
}

void ALStep::FlagMod_Implementation(const FName& FlagName, const float Diff, const float Total) {
	CheckFlagsFinish();
}

void ALStep::CheckItemsFinish() {
	const int32 NumItems = ItemsFinish.Num();
	if (NumItems<=0) return;

	for (int32 i=0; i<NumItems; ++i) {
		if (!Inventory->Has(ItemsFinish[i])) return;
	}

	FinishAfterDlgs();
}

void ALStep::CheckFlagsFinish() {
	const int32 NumItems = FlagsFinish.Num();
	if (NumItems<=0) return;

	for (int32 i=0; i<NumItems; ++i) {
		if (!Flags->Has(ItemsFinish[i])) return;
	}

	FinishAfterDlgs();
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
	if (LIKELY(IsValid(Diags))) {
		Diags->OnDone.RemoveAll(this);
		Diags->OnShow.RemoveAll(this);
	}
	Diags = nullptr;

	if (LIKELY(IsValid(Inventory))) Inventory->OnMod.RemoveAll(this);
	Inventory = nullptr;
	
	if (LIKELY(IsValid(FB))) FB->OnChange.RemoveAll(this);
	FB = nullptr;

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
	if (LIKELY(Diags)) {
		// avoid possible double triggering. since finish is called from several origins
		Diags->OnDone.RemoveDynamic(this, &ALStep::Finish);
		Diags->OnShow.RemoveDynamic(this, &ALStep::DlgShow);
	}
	Super::Finish_Implementation();
}

void ALStep::SetActorsShowActive(const bool Active, const bool WithFade) {
	for (AActor* const A: ActorsShow) {
		if (UNLIKELY(!IsValid(A))) continue;

		ALInteract* const Inter = Cast<ALInteract>(A);
		if (!Inter || !WithFade) { // hiding it with fades will be jarring
			A->SetActorHiddenInGame(!Active);
			// fade will call set-active. otherwise have to call it manually.
			if (Inter) Inter->SetActive(Active);
			// Avoid calling 'Fade' twice, just in case there are side effects.
			continue;
		}
		Inter->Fade(Active, true);
	}
}

void ALStep::DoIntersActiveAny(const TArray<TObjectPtr<AInteract>>& A, const bool NewActive) {
	for (AInteract* const I: A) {
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

void ALStep::DoIntersFade(const TArray<ALInteract*>& A, const bool In) {
	for (ALInteract* const I: A) {
		if (LIKELY(IsValid(I))) I->Fade(In);
	}
}

void ALStep::DoIntersTrigger() const {
	for (const TObjectPtr<ALInteract>& I: IntersTrigger) {
		if (LIKELY(IsValid(I))) I->TryTrigger();
	}
}

void ALStep::DoIntersHint() const {
	for (const TObjectPtr<AInteract>& I: IntersHint) {
		if (LIKELY(IsValid(I))) I->UseHint = true;
	}
}
