// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI03.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APuzzleI03::APuzzleI03():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 0, 3, 0}; // F.A.W.N. // TODO

	static FName DoneId = "PZ03_T";
	DoneDlg = DoneId;
	DoneFB = .15;
}

void APuzzleI03::PostLoad() {
	Super::PostLoad();
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ03xC00"},
		{LDConsts::Items::Card1, "PZ03xC01"},
		{LDConsts::Items::Card2, "PZ03xC02"},
		{LDConsts::Items::Card3, "PZ03xC03"}
	};
	SetUseItemDlgs(Dlgs);

	static const TArray<bool> Locks = {false, false, false, false}; // TODO
	SetLocks(Locks);
}

void APuzzleI03::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 2, 5, 0}; // TODO
	SetStates(States);
}


