// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI06.h"

#include "Interact/CPuzzle.h"
#include "Inventory/Flags.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Core/Settings/LSettings.h"

#define _myclass_ TEXT("APuzzleI06")

APuzzleI06::APuzzleI06():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 1, 2};
	ResetOnFail = false;

	static FName DoneId = "PZ06_T";
	DoneDlg = DoneId;
	DoneFB = .1;
}

void APuzzleI06::PostLoad() {
	Super::PostLoad();
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ06xC00"},
		{LDConsts::Items::Card1, "PZ06xC01"},
		{LDConsts::Items::Card2, "PZ06xC02"}
	};
	SetUseItemDlgs(Dlgs);

	const TArray<bool> Locks = {false, false, false};
	SetLocks(Locks);
}

void APuzzleI06::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {1, 2, 0};
	SetStates(States);

	// this is the only safe place to set active and get the settings
	const ULSettings* const Settings = ULSettings::Instance(this);
	const EFeat& ChapFeat = Settings ? Settings->CurrentChapterFeat() : EFeat::NONE;
	const bool Active = ChapFeat == EFeat::C_03; // disabled manually. still needs work.
	SetActives(Active);
	UE_LOG(LogTemp, Log, TEXT("PuzzleI06::%hs Active=%i"), __func__, Active);
	
	const UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;

	constexpr float DiffAm = .3;
	const float Diff = FMath::Lerp(-DiffAm, +DiffAm,
		Flags->Get(LDConsts::Flags::Settings::Global::Foxy));
	DoneFB += Diff;
	UE_LOG(LogTemp, Log, TEXT("%s::%hs foxify by=%.4f"),
		_myclass_, __func__,  Diff);
}
