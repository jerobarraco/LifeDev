// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI05.h"

#include "Interact/CPuzzle.h"
#include "JUtils/Misc/JMiscConsts.h"
// #include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI05::APuzzleI05():Super() {
	UseHint = true;
	RewardFlash = .15;
	// TODO
	// RewardActor = TSoftObjectPtr<AInteract>(FSoftObjectPath(
		// "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card03_UAID_D8BBC116E501EAD901_1338864495"));

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
	// {0, 2, 2, 4, 6, 0}
	static const TArray<int32> States = {0, 2, 2, 4, 6, 0}; // TODO x, z, x, z, z, x
	SetStates(States);

	// static const TArray<bool> Locks = {false, true, false, true, true, false};
	static const TArray<bool> Locks = {false, false, false, false, false, false};
	SetLocks(Locks);
	SetHiddensInGame(false);
}
