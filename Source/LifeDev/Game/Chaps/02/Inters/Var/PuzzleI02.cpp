// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI02.h"

#include "Interact/CPuzzle.h"
#include "JUtils/Misc/JMiscConsts.h"

#include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI02::APuzzleI02():Super() {
	UseHint = true;
	RewardFlash = .15;
	RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube;
	// static FName DoneId = "PZ02_T";
	// TriggerDlg = DoneId;
	RewardActor = TSoftObjectPtr<ALInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card02_UAID_D8BBC116E501DFC401_1977970429"));

	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 0, 3}; // S.A.D.
	CPuzzle->SetPieces({
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube03_UAID_D8BBC116E501E2CD01_1200587439"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501E2CD01_1181927438"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501E2CD01_1176815437"),
	});
}

void APuzzleI02::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 2, 5};
	SetStates(States);
	static const TArray<bool> Locks = {true, false, false};
	SetLocks(Locks);
	// if the player has reached here without doing the puzzle, hint.
	SetUseHint(true);
	SetHintConditions("{Step.Start.C2S4}");
}


