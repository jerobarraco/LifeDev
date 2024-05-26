// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI03.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Sys/Consts/ConstFlags.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APuzzleI03::APuzzleI03():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 1, 0, 0, 4}; // S.H.A.M.E

	static FName DoneId = "PZ03_T";
	DoneDlg = DoneId;
	DoneFB = .15;
	DoneFlag = LDConsts::Flags::Stats::PuzzlesCube;
}

void APuzzleI03::PostLoad() {
	Super::PostLoad();
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ03xC00"},
		{LDConsts::Items::Card1, "PZ03xC01"},
		{LDConsts::Items::Card2, "PZ03xC02"}
	};
	SetUseItemDlgs(Dlgs);

	static const TArray<bool> Locks = {true, false, true, false, false};
	SetLocks(Locks);
}

void APuzzleI03::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 4, 0, 3, 2};
	SetStates(States);
}


