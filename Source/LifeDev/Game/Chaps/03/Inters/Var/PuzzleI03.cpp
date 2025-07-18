// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI03.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI03::APuzzleI03():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 1, 0, 0, 4}; // S.H.A.M.E

	// static FName DoneId = "PZ03_T";
	// TriggerDlg = DoneId;
	RewardFlash = .15;
	RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube;
	RewardActor = TSoftObjectPtr<AInteract>(FSoftObjectPath(
		"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card03_UAID_D8BBC116E501EAD901_1338864495"));
}

void APuzzleI03::PostLoad() {
	Super::PostLoad();

	static const TArray<bool> Locks = {true, false, true, false, false};
	SetLocks(Locks);
}

void APuzzleI03::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 4, 0, 3, 2};
	SetStates(States);
}
