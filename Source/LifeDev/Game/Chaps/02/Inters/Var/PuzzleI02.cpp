// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI02.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Sys/Consts/ConstFlags.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APuzzleI02::APuzzleI02():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 0, 3}; // S.A.D

	static FName DoneId = "PZ02_T";
	DoneDlg = DoneId;
	DoneFB = .15;
	DoneFlag = LDConsts::Flags::Stats::PuzzlesCube;
}

void APuzzleI02::PostLoad() {
	Super::PostLoad();
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ02xC00"},
		{LDConsts::Items::Card1, "PZ02xC01"},
		{LDConsts::Items::Card2, "PZ02xC02"}
	};
	SetUseItemDlgs(Dlgs);

	static const TArray<bool> Locks = {true, false, false};
	SetLocks(Locks);
}

void APuzzleI02::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 2, 5};
	SetStates(States);
}


