// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI04.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APuzzleI04::APuzzleI04():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {1, 2, 0}; 

	static FName DoneId = "PZ04_T";
	DoneDlg = DoneId; // really? TODO maybe not necessary
	// DoneFB = .15;
	// TODO this will get the engine stuck
	DoneStep = "C3S0"; // TODO trigger boss
}

void APuzzleI04::PostLoad() {
	Super::PostLoad();
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ04xC00"},
		{LDConsts::Items::Card1, "PZ04xC01"},
		{LDConsts::Items::Card2, "PZ04xC02"},
		{LDConsts::Items::Card3, "PZ04xC03"}
	};
	SetUseItemDlgs(Dlgs);

	static const TArray<bool> Locks = {false, false, false, false};
	SetLocks(Locks);
}
//
// void APuzzleI04::BeginPlay() {
// 	Super::BeginPlay();
//
// 	static const TArray<int32> States = {0, 0, 0};
// 	SetStates(States);
// }
//

