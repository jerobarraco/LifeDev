// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI06.h"

#include "Interact/CPuzzle.h"
#include "Inventory/Flags.h"
#include "Story/Story.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Game/Chaps/03/Steps/LStepC3S000.h"

#define _myclass_ TEXT("APuzzleI06")

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

	// TODO fix. this logic is not good. as it can be spawned in a previous chapter.
	// ideally i would add it to the step, but since the stepc3s0 is always loaded (for skipping)
	// i can't add this puzzle since it exists on other datalayers.

	if (LIKELY(Flags)) {
		constexpr float DiffAm = .3;
		const float Diff = FMath::Lerp(-DiffAm, +DiffAm,
			Flags->Get(LDConsts::Flags::Settings::Global::Foxy));
		RewardFlash += Diff;
		UE_LOG(LogTemp, Log, TEXT("%s::%hs foxify by=%.4f"),
			_myclass_, __func__, Diff);
	}
	
	if (LIKELY(Story)) {
		Story->OnStart.AddUniqueDynamic(this, &APuzzleI06::StepStarted);
		StepStarted(Story->GetStep(Story->GetCurrent())); // attempt to fix not usable when launching directly
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

void APuzzleI06::StepStarted(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	if (LIKELY(Step->Name != ALStepC3S000::SName)) return;

	SetActives(true);

	// no need to listen anymore.
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);

	// i would prefer to have something more direct instead of listening for every step.
	// Ideally this would happen on the Step, but i can't reference this actor
	// from the step since, it just so happens that steps C*S0 needs to be preloaded in order to skip to any
	// and this actor belongs in a datalayer.
}
