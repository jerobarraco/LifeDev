// Copyright Jerónimo Barraco-Mármol

#include "LPuzzle.h"

#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"
#include "Interact/Interact.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "JUtils/Misc/JUtilsMisc.h"

#include "Story/Story.h"

#include "LifeDev/Core/Consts/ConstDlgs.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/LInteract.h"

ALPuzzle::ALPuzzle():Super() {
	UseAnim = false;

	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
	CPuzzle->DisableOnDone = true;

	SetAutoActivate(false); // by default i'm not using this puzzle as directly.
	SetMobility(EComponentMobility::Type::Static);
}

void ALPuzzle::BeginPlay() {
	Super::BeginPlay();

	if (LIKELY(IsValid(CPuzzle))) {
		CPuzzle->OnDone.AddUniqueDynamic(this, &ALPuzzle::Done);
		CPuzzle->OnUpdate.AddUniqueDynamic(this, &ALPuzzle::Update);
		CPuzzle->OnReset.AddUniqueDynamic(this, &ALPuzzle::DoReset);
	} else {
		UE_LOG(LogTemp, Warning,
			TEXT("CPuzzle in Puzzle is not valid!! Huge problem. o=%s"),
			*GetNameSafe(this));
	}
	
	if (IsValid(DoneActor)) { // TODO nuke
		DoneActor->SetActorHiddenInGame(true);
		AInteract* const Reward = Cast<AInteract>(DoneActor);
		if (IsValid(Reward)) Reward->SetActive(false);
	}
}

void ALPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ClearTimer(); // uses cpuzzle. do first.

	if (LIKELY(CPuzzle)) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
		CPuzzle->OnReset.RemoveAll(this);
	}
	CPuzzle = nullptr;


	Super::EndPlay(EndPlayReason);
}

void ALPuzzle::SetUseItemDlgs(const TMap<FName, FName>& Dlgs) const{
	if (UNLIKELY(!CPuzzle)) return;

	// set the dialogs on each registered interact
	TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	for (AInteract* const I: Inters) {
		ALInteract* const LI = Cast<ALInteract>(I);
		if (UNLIKELY(!IsValid(LI))) continue;
		// better to override the whole array than having issues down the line.
		// it also allows to remove stuff.
		LI->UseItemDlgs = Dlgs;
	}
}

void ALPuzzle::Reset() {
	Super::Reset();
	ClearTimer();

	if (LIKELY(CPuzzle)) CPuzzle->Reset(); // triggers a delegate
}

void ALPuzzle::ClearTimer() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	
	W->GetTimerManager().ClearTimer(ResetTimer);
	ResetTimer.Invalidate();
}

void ALPuzzle::Done_Implementation(const bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("ALPuzzle::Done ok=%i o=%s"),
		IsOk, *GetNameSafe(this));

	// TODO
	// Super::Done_Implementation(IsOk); // triggers the interact AND RESETS (next frame)
	
	if (!IsOk) return; // ok to skip super on not ok, since super doesn't care

	/// do all rewardy stuff

	FString Label;
	UJUtilsMisc::ObjectLabel(this, Label);
	
	const FName DoneId(LDConsts::Dlgs::Inter::PuzzleDonePre + Label);

	if (LIKELY(Flashback)) Flashback->ModVal(DoneFB);
	if (LIKELY(Flags)) {
		Flags->Mod(DoneFlag, 1); // intentionally ADDING one (not setting to one)
		Flags->Mod(DoneId, 1); // intentionally ADDING one (not setting to one)
	}
	// intentionally ADDING one (not setting to one)
	if (LIKELY(Inventory)) Inventory->Mod(DoneItem, 1);

	// doing dialog after the flags, since now the dialog system can read the flags and items on the condition
	if (LIKELY(Diags)) Diags->AddId(DoneDlg) || Diags->AddId(DoneId);

	// story step at end to not break other stuff much.
	if (LIKELY(Story) && !DoneStep.IsNone()) Story->StartNext(DoneStep);

	// TODO This should be implemented on the LInteract on trigger. need to make sure all children are set up correctly.
	// fade if it's an L interact (those can fade)
	// a bit yucky but better than subclassing cpuzzle. it's actually quite the best option.
	ALInteract* const Reward = Cast<ALInteract>(DoneActor);
	if (IsValid(Reward)) Reward->Fade(true);
}

