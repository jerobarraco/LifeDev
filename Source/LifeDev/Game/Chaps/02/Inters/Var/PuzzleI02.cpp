// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI02.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI02::APuzzleI02():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 0, 3}; // S.A.D.

	UseHint = true;
	RewardFlash = .15;
	RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube;
	// static FName DoneId = "PZ02_T";
	// TriggerDlg = DoneId;
	RewardActor = TSoftObjectPtr<ALInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card02_UAID_D8BBC116E501DFC401_1977970429"));
}

void APuzzleI02::PostLoad() {
	Super::PostLoad();

	static const TArray<bool> Locks = {true, false, false};
	SetLocks(Locks);
}

void APuzzleI02::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 2, 5};
	SetStates(States);
}


