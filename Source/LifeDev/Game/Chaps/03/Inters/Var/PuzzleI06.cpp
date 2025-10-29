// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI06.h"

#include "Interact/CPuzzle.h"
#include "Inventory/Flags.h"
#include "JUtils/Misc/JMiscConsts.h"
#include "Story/Story.h"

#include "LifeDev/Core/Consts/ConstFlags.h"

#define _myclass_ TEXT("APuzzleI06")

// this puzzle gets activated by the step (c3s0)

APuzzleI06::APuzzleI06():Super() {
	UseHint = true;

	ResetOnFail = false;
	UseAutoActivate = false; // This is activated by the step c3s0

	// static FName DoneId = "PZ06_T";
	// TriggerDlg = DoneId;
	RewardFlash = .1;
	RewardActor = TSoftObjectPtr<AInteract>(FSoftObjectPath(
		"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.TapeI03_UAID_D8BBC116E501B80902_1106536415"));
	RewardIntersHint = { // plant
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E501AFD801_1178397079"))
	};

	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 1, 2};
	CPuzzle->SetPieces({
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E50171C601_2091017155"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E50172C601_1518888333"),
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E50172C601_1509496332"),
	});
}

void APuzzleI06::BeginPlay() {
	Super::BeginPlay();

	/// apply these here since the cpuzzle pieces might not be loaded in the constructor.
	
	static const TArray<int32> States = {1, 2, 0};
	SetStatesNow(States);

	const TArray<bool> Locks = {false, false, false};
	SetLocks(Locks);
	SetAutoActives(true);
	SetActives(true);
	// SetUseHints(true);

	if (LIKELY(Flags)) {
		constexpr float DiffAm = .3;
		const float Diff = FMath::Lerp(-DiffAm, +DiffAm,
			Flags->Get(LDConsts::Flags::Settings::Global::Foxy));
		RewardFlash += Diff;
		UE_LOG(LogTemp, Log, TEXT("%s::%hs foxify by=%.4f"),
			_myclass_, __func__, Diff);
	}
}

void APuzzleI06::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}
