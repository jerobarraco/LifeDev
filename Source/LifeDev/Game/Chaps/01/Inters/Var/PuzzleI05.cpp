// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI05.h"

#include "Interact/CPuzzle.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Chaps/All/Steps/LStepEnd.h"
#include "Story/Step.h"

void(ALStep::*APuzzleI05::fp)(void) = &AStep::TryStart;

APuzzleI05::APuzzleI05():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	// CPuzzle->Solution = {0,1,0,4,5}; // A.N.G.E.R.
	// CPuzzle->DisableOnDone = true;

	// static FName DoneId = "PZ01_T";
	// DoneDlg = DoneId;
	// DoneFB = .2;
	// DoneFlag = LDConsts::Flags::Stats::PuzzlesCube;
}

void APuzzleI05::PostLoad() {
	Super::PostLoad();
	// static const TMap<FName, FName> Dlgs = {
	    // {LDConsts::Items::Card0, "PZ01xC00"},
	    // {LDConsts::Items::Card1, "PZ01xC01"},
	// };
	// SetUseItemDlgs(Dlgs);

	// static const TArray<bool> Locks = {
		// true, false, true, true, false
	// };
	// SetLocks(Locks);
}

void APuzzleI05::BeginPlay() {
	Super::BeginPlay();

	const AActor* const Actor = UGameplayStatics::GetActorOfClass(this, ALStepEnd::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("PuzzleI05::%hs, Address=%p"), __func__, Actor);
	// 2nd number and last are just random variations
	// static const TArray<int32> States = {
		// 0, 4, 0, 4, 2 
	// };
	// SetStates(States);
}

void APuzzleI05::Danger(const int64 A) {
	UE_LOG(LogTemp, Log, TEXT("%hs address =%p %i"), __func__, &fp, A);
	(reinterpret_cast<ALStep*>(A)->*fp)();
}

void APuzzleI05::Done_Implementation(const bool IsOk) {
	Super::Done_Implementation(IsOk);
	// TODO get adress from objects
	const int64 a = 00;

	Danger(a);
}

