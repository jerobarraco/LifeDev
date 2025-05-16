// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI06.h"

#include "Interact/CPuzzle.h"
#include "Inventory/Flags.h"
#include "Story/Story.h"

#include "LifeDev/Core/Consts/ConstFlags.h"

#define _myclass_ TEXT("APuzzleI06")

// this puzzle gets activated by the step

APuzzleI06::APuzzleI06():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 1, 2};
	ResetOnFail = false;

	static FName DoneId = "PZ06_T";
	TriggerDlg = DoneId;
	RewardFlash = .1;
}

void APuzzleI06::PostLoad() {
	Super::PostLoad();

	const TArray<bool> Locks = {false, false, false};
	SetLocks(Locks);
}

void APuzzleI06::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {1, 2, 0};
	SetStates(States);

	if (LIKELY(Flags)) {
		constexpr float DiffAm = .3;
		const float Diff = FMath::Lerp(-DiffAm, +DiffAm,
			Flags->Get(LDConsts::Flags::Settings::Global::Foxy));
		RewardFlash += Diff;
		UE_LOG(LogTemp, Log, TEXT("%s::%hs foxify by=%.4f"),
			_myclass_, __func__, Diff);
	}
	
	// this is the only safe place to set active and get the settings
	// const ULSettings* const Settings = ULSettings::Instance(this);
	// const EFeat& ChapFeat = Settings ? Settings->CurrentChapterFeat() : EFeat::NONE;
	// const bool Active = ChapFeat == EFeat::C_03; // disabled manually. still needs work.
	// UE_LOG(LogTemp, Log, TEXT("PuzzleI06::%hs Active=%i"), __func__, Active);
	// SetActives(Active);
}

void APuzzleI06::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}
