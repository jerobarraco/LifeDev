// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI01.h"

#include "Interact/CPuzzle.h"

APuzzleI01::APuzzleI01():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0,1,0,4,5}; // A.N.G.E.R.
	CPuzzle->DisableOnDone = true;

	// don't do this, the fb is at .85, and we use the remaining fb to animate the camera.
	// the step has a FbAnimTo 1
	// RewardFlash = .2;
	// RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube; // i could move this to a base class for cube puzzles
	// static FName DoneId = "PZ01_T";
	// TriggerDlg = DoneId;
	UseHint = true;
	UseAutoActivate = true;
	RewardActor = TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card01_UAID_D8BBC116E50181BA01_2066401320"));
}

void APuzzleI01::BeginPlay() {
	Super::BeginPlay();

	// 2nd number and last are just random variations
	static const TArray<int32> States = { 0, 4, 0, 4, 2 };
	SetStates(States);
	static const TArray<bool> Locks = { true, false, true, true, false };
	SetLocks(Locks);
	SetHiddensInGame(true);
}
