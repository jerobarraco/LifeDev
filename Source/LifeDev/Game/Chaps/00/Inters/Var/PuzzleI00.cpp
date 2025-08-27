// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI00.h"

#include "Interact/CPuzzle.h"
#include "JUtils/Misc/JMiscConsts.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI00::APuzzleI00():Super() {
	RewardFlash = .2;
	RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube; // i could move this to a base class for cube puzzles
	// static FName DoneId = "PZ00_T";
	// TriggerDlg = DoneId;
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {5,4,0,5}; // F.E.A.R.
	CPuzzle->SetPieces({
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E5014BCE01_1741001916"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube02_UAID_D8BBC116E5014BCE01_1747714917"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube02_UAID_D8BBC116E5014BCE01_1753569918"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube03_UAID_D8BBC116E5014BCE01_1810824921")
	});
}

void APuzzleI00::BeginPlay() {
	Super::BeginPlay();
	static const TArray<int32> States = { 5,0,0,0 };
	SetStates(States);
	SetHiddensInGame(true); // start hidden
	static const TArray<bool> Locks = { true, false, true, false };
	SetLocks(Locks);
}
