// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI05.h"

#include "Interact/CPuzzle.h"
#include "JUtils/Misc/JMiscConsts.h"
// #include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI05::APuzzleI05():Super() {
	UseHint = true;
	// static FName DoneId = "PZ03_T";
	// TriggerDlg = DoneId;
	RewardFlash = .15;
	// RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube;
	RewardActor = TSoftObjectPtr<AInteract>(FSoftObjectPath(
		"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card03_UAID_D8BBC116E501EAD901_1338864495"));

	CPuzzle->Type = EPuzzleType::COMBINATION;
	// TODO "accept"
	// CPuzzle->Solution = {0, 1, 0, 0, 4}; // S.H.A.M.E
	// CPuzzle->SetPieces({
	// 	SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube04_UAID_D8BBC116E501EDD901_1493147028"),
	// 	SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube02_UAID_D8BBC116E501EDD901_1351720023"),
	// 	SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501EDD901_1421491025"),
	// 	SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube03_UAID_D8BBC116E501EDD901_1442181026"),
	// 	SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.WCube01_UAID_D8BBC116E501EDD901_1487611027"),
	// });
}

void APuzzleI05::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 4, 0, 3, 2}; // TODO
	SetStates(States);

	static const TArray<bool> Locks = {true, false, true, false, false}; // TODO
	SetLocks(Locks);
	SetHiddensInGame(true);
}
