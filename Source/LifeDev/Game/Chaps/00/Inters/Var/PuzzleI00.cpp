// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI00.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APuzzleI00::APuzzleI00():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {5,4,0,5}; // F.E.A.R.

	static FName DoneId = "PZ00_T";
	DoneDlg = DoneId;
	DoneFB = .2;
}

void APuzzleI00::PostLoad() {
	// by now the interacts set in editor are loaded
	Super::PostLoad();
	if (!CPuzzle) return;

	static const TMap<FName, FName> Dlgs = {
        {LDConsts::Items::Card0, "PZ00xC00"}
    };
    SetUseItemDlgs(Dlgs);
}
