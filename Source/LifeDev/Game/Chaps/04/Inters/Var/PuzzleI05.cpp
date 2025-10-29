// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI05.h"

#include "Interact/CPuzzle.h"
#include "JUtils/Misc/JMiscConsts.h"

APuzzleI05::APuzzleI05():Super() {
	UseHint = true;
	RewardFlash = .15;
	RewardActor = SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card04_UAID_D8BBC116E501FB3602_1428206695");

	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 2, 2, 4, 3, 1}; // A.C.C.E.P.T.
	CPuzzle->SetPieces({
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501A29402_1690365296"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501A29402_1695862297"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501A29402_1699051298"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501A39402_1323041478"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube03_UAID_D8BBC116E501A29402_1773873301"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube04_UAID_D8BBC116E501A29402_1713043300"),
	});
}

void APuzzleI05::BeginPlay() {
	Super::BeginPlay();

	/// apply these here since the cpuzzle pieces might not be loaded in the constructor.
	
	static const TArray<int32> States = {3, 2, 5, 4, 3, 3}; // {0, 2, 2, 4, 3, 1}
	SetStatesNow(States);

	static const TArray<bool> Locks = {false, true, false, true, true, false};
	SetLocks(Locks);
	SetHiddensInGame(true);
	SetUseHints(true); // start with hints
}
