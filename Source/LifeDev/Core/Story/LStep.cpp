// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Diags/Diags.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "Story/Story.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/LInteract.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLStoryStep, Log, Log);

void ALStep::Start_Implementation() {
	Super::Start_Implementation();

	EnsureItems(); // make sure items are awarded
}

void ALStep::Stop_Implementation() {
	if (IsValid(Diags)) Diags->OnDone.RemoveAll(this);
	if (IsValid(Inventory)) Inventory->OnMod.RemoveAll(this);
	if (IsValid(FB)) FB->OnChange.RemoveAll(this);

	if (IsValid(Actor)) {
		ALInteract* const Inter = Cast<ALInteract>(Actor);
		if (Inter) Inter->Fade(false);
	}
	if (IsValid(Ghosts)) {
		Ghosts->SetPlaying(false);
	}

	UWorld* const W = GetWorld();
	if (W) {
		// ensure we don't double trigger
		W->GetTimerManager().ClearAllTimersForObject(this);
		// Destroy them during the fade
		FTimerHandle H;
		W->GetTimerManager().SetTimer(H, this, &ALStep::DestroyActors, 2);
	}

	RemoveItems();
	
	Super::Stop_Implementation();
}

void ALStep::PostWait_Implementation() {
	Super::PostWait_Implementation();
	UWorld* const W = GetWorld();
	if (!W) return;


	// check items. do on postWait to avoid possibly finishing the step while it's starting.
	if (!ItemsFinish.IsEmpty()) {
		Inventory->OnMod.AddUniqueDynamic(this, &ALStep::ItemMod);
		
		// ensure to check if we already have the item
		W->GetTimerManager().SetTimerForNextTick(this, &ALStep::CheckItemsFinish);
	}

	AGameModeBase* const GameModeBase = W->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get(); // doesn't work
	if (IsValid(LGGameMode)) {
		// do only on postwait. otherwise the input is reset before it faded out.
		LGGameMode->SetCharInputEnabled(InputEnabled);
	}

	Ghosts = Cast<AGhosts>(W->SpawnActor(AGhosts::StaticClass()));
	if (IsValid(Ghosts)) {
		Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Ghosts->SetActorRelativeLocation(GhostPos);
		Ghosts->SetPlaying(true);
	}

	if (IsValid(Actor)) {
		Actor->SetActorHiddenInGame(false);
		ALInteract* const Inter = Cast<ALInteract>(Actor);
		if (Inter) Inter->Fade(false);
	}

	// show dialogs
	StartDialogs();
}

void ALStep::StartDialogs() {
	if (DlgId.IsNone()) return;

	Diags->OnShow.AddUniqueDynamic(this, &ALStep::DlgShow);
	Diags->AddId(DlgId);
	FinishAfterDlgs();
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
	if (IsValid(Actor)) Actor->Destroy();
	Actor = nullptr;

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
	if (FB) FB->ModVal(FbDiagMod);
}

void ALStep::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!World) return;
	Diags = World->GetSubsystem<UDiags>();
	Inventory = World->GetSubsystem<UInventory>();
	FB = World->GetSubsystem<UFlashback>();
	Flags = UFlags::Instance(World);
	
	if (IsValid(Actor)) {
		Actor->SetActorHiddenInGame(true);
		ALInteract* const Inter = Cast<ALInteract>(Actor);
		if (Inter) Inter->SetEnabled(false);
	}
}

void ALStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(Diags)) Diags->OnDone.RemoveAll(this);
	Diags = nullptr;

	if (IsValid(Inventory)) Inventory->OnMod.RemoveAll(this);
	Inventory = nullptr;
	
	if (IsValid(FB)) FB->OnChange.RemoveAll(this);
	FB = nullptr;

	// always at end
	Super::EndPlay(EndPlayReason);
}

void ALStep::PostLoad() {
	Super::PostLoad();
	if (UseFadeTime) {
		UStory* const Story = UStory::Instance(GetWorld());
		WaitTime = Story ? Story->FadeTime : 1; 
	}
	
	// avoid finishing earlier if we have Diags
	if (!DlgId.IsNone()) {
		FinishPostWait = false;
	}
}

void ALStep::Finish_Implementation() {
	// avoid possible double triggering. since finish is called from several origins
	Diags->OnDone.RemoveDynamic(this, &ALStep::Finish);
	Super::Finish_Implementation();
}


// todo fix c0s1 c1s2 c2s1 c3s1