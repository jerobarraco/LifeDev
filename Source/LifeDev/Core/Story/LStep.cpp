// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Diags/Diags.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "Story/Story.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/LInteract.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLStoryStep, Log, Log);

void ALStep::TryStart_Implementation() {
	Super::TryStart_Implementation();

	EnsureItems(); // make sure items are awarded
}

void ALStep::Stop_Implementation() {
	if (IsValid(Diags)) Diags->OnDone.RemoveAll(this);
	if (IsValid(Inventory)) Inventory->OnMod.RemoveAll(this);
	if (IsValid(FB)) FB->OnChange.RemoveAll(this);
	if (IsValid(Ghosts)) Ghosts->SetPlaying(false);

	SetShowActorEnabled(false, true);
	SetShowActorsEnabled(false, true);
	SetIntersEnabled(false);
	DoIntersFade(IntersFadeOut, false);
	RemoveItems();

	UWorld* const W = GetWorld();
	if (W) {
		if (UseRain) ALMusicMan::SetRainS(W, false);

		// ensure we don't double trigger
		W->GetTimerManager().ClearAllTimersForObject(this);
		// Destroy them during the fade
		FTimerHandle H;
		W->GetTimerManager().SetTimer(H, this, &ALStep::DestroyActors, 2);
	}

	Super::Stop_Implementation(); // do at end.
}

void ALStep::Start_Implementation() {
	Super::Start_Implementation();
	UWorld* const W = GetWorld();
	if (!W) return;

	// check items. do on postWait to avoid possibly finishing the step while it's starting.
	if (!ItemsFinish.IsEmpty()) {
		Inventory->OnMod.AddUniqueDynamic(this, &ALStep::ItemMod);
		
		// ensure to check if we already have the item. but not now to not affect the flow of child classes
		W->GetTimerManager().SetTimerForNextTick(this, &ALStep::CheckItemsFinish);
	}

	AGameModeBase* const GameModeBase = W->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get(); // doesn't work
	
	// do only on postwait. otherwise the input is reset before it faded out.
	if (IsValid(LGGameMode)) LGGameMode->SetCharInputEnabled(InputEnabled);

	if (UseGhosts) {
		Ghosts = Cast<AGhosts>(W->SpawnActor(AGhosts::StaticClass()));
		if (IsValid(Ghosts)) {
			Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Ghosts->SetActorRelativeLocation(GhostPos);
			Ghosts->SetPlaying(true);
		}
	}

	if (UseRain) ALMusicMan::SetRainS(W, true);

	SetShowActorEnabled(true, true);
	SetShowActorsEnabled(true, true);
	SetIntersEnabled(true);
	DoIntersFade(IntersFadeIn, true);
	DoIntersTrigger();

	// show dialogs
	StartDialogs();
}

void ALStep::StartDialogs() {
	if (DlgId.IsNone()) return;

	SetFBDiagAuto();
	Diags->OnShow.AddUniqueDynamic(this, &ALStep::DlgShow);
	Diags->AddId(DlgId);
	FinishAfterDlgs();
}

void ALStep::SetFBDiagAuto() {
	if (!UseFBDiagAuto) return;

	FDialogSequence Seq;
	int32 Len = 0;
	const bool Ok = Diags->GetSeq(DlgId, Seq);
	if (Ok) {
		Len = Seq.DiagRows.Num();
	} else {
		FDialog D;
		FDialogChar C;
		const bool Ok2 = Diags->GetDiag(DlgId, D, C);
		if (Ok2) Len = 1;
	}
	
	// avoid division by 0, but also makes no sense otherwise.
	if (Len <= 0 ) return;
	// TODO there might be an issue here. check if GetValTo or GetVal is the correct.
	const float FBCurrent = FB->GetValTo();
	FBDiagMod = (FBDiagAutoTo - FBCurrent) / Len;

	UE_LOG(LogLStoryStep, Log, TEXT("%hs DiagMod=%f, DiagAutoTo=%f, Current=%f, Len=%i"),
		__func__, FBDiagMod, FBDiagAutoTo, FBCurrent, Len);
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
	UE_LOG(LogLStoryStep, Log, TEXT("Destroy actors called"));
	// this is a bit dangerous, we can't go back to chap 0 without reloading.
	// but also more performant.
	if (IsValid(ShowActor)) ShowActor->Destroy();
	ShowActor = nullptr;

	for (AActor* const A: ActorsShow) {
		if (IsValid(A)) A->Destroy();
	}
	ActorsShow.Empty();

	if (IsValid(Ghosts)) Ghosts->Destroy();
	Ghosts = nullptr;
}

void ALStep::ItemMod(const FName& ItemName, int32 Diff, const FItem& Item) {
	CheckItemsFinish();
}

void ALStep::CheckItemsFinish() {
	const int32 NumItems = ItemsFinish.Num();
	if (NumItems<=0) return;

	for (int32 i=0; i<NumItems; ++i) {
		if (!Inventory->Has(ItemsFinish[i])) return;
	}

	FinishAfterDlgs();
}

void ALStep::DlgShow_Implementation(const FDialog& Diag) {
	if (FB) FB->ModVal(FBDiagMod); // no need to check for IsNearlyZero. modval does it.
}

void ALStep::BeginPlay() {
	Super::BeginPlay();

	const UWorld* const World = GetWorld();
	if (!World) return;

	Diags = World->GetSubsystem<UDiags>();
	Inventory = World->GetSubsystem<UInventory>();
	FB = World->GetSubsystem<UFlashback>();
	Flags = World->GetSubsystem<UFlags>();

	SetShowActorEnabled(false, false);
	SetShowActorsEnabled(false, false);
	SetIntersEnabled(false);
}

void ALStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(Diags)) {
		Diags->OnDone.RemoveAll(this);
		Diags->OnShow.RemoveAll(this);
	}
	Diags = nullptr;

	if (IsValid(Inventory)) Inventory->OnMod.RemoveAll(this);
	Inventory = nullptr;
	
	if (IsValid(FB)) FB->OnChange.RemoveAll(this);
	FB = nullptr;

	Super::EndPlay(EndPlayReason); // always at end
}

void ALStep::PostLoad() {
	Super::PostLoad();

	if (UseFadeTime) {
		const UStory* const Story = UStory::Instance(GetWorld());
		WaitTime = Story ? Story->FadeTime : 1; 
	}
	
	// avoid finishing earlier if we have Diags
	if (!DlgId.IsNone()) FinishPostWait = false;
}

void ALStep::Finish_Implementation() {
	if (Diags) {
		// avoid possible double triggering. since finish is called from several origins
		Diags->OnDone.RemoveDynamic(this, &ALStep::Finish);
		Diags->OnShow.RemoveDynamic(this, &ALStep::DlgShow);
	}
	Super::Finish_Implementation();
}

void ALStep::SetShowActorEnabled(const bool Enabled, const bool WithFade) {
	if (!IsValid(ShowActor)) return;

	ShowActor->SetActorHiddenInGame(!Enabled);
	ALInteract* const Inter = Cast<ALInteract>(ShowActor);
	if (!Inter) return;

	if (WithFade) Inter->Fade(Enabled);
	// fade will call set-enabled. otherwise have to call it manually.
	// make sure to call it. Avoid calling twice just in case there are side effects.
	else Inter->SetEnabled(Enabled);
}

void ALStep::SetShowActorsEnabled(const bool Enabled, const bool WithFade) {
	for (AActor* const A: ActorsShow) {
		if (!IsValid(A)) continue;
		
		A->SetActorHiddenInGame(!Enabled);
		ALInteract* const Inter = Cast<ALInteract>(A);
		if (!Inter) continue;

		if (WithFade) Inter->Fade(Enabled);
		// fade will call set-enabled. otherwise have to call it manually.
		// make sure to call it. Avoid calling twice just in case there are side effects.
		else Inter->SetEnabled(Enabled);
	}
}

void ALStep::DoIntersFade(const TArray<ALInteract*>& A, const bool In) {
	for (ALInteract* const I: A) {
		if (IsValid(I)) I->Fade(In);
	}
}

void ALStep::SetIntersEnabled(const bool Enabled) {
	for (AInteract* const I: IntersEnable) {
		if (IsValid(I)) I->SetEnabled(Enabled);
	}
}

void ALStep::DoIntersTrigger() const {
	for (AInteract* const I: IntersTrigger) {
		if (IsValid(I)) I->TryTrigger();
	}
}
