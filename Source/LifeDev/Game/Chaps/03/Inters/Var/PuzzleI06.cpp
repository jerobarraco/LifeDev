// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI06.h"

#include "Interact/CPuzzle.h"
#include "Inventory/Flags.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"

APuzzleI06::APuzzleI06():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {1, 2, 0}; // TODO
	ResetOnFail = false;

	static FName DoneId = "PZ06_T";
	DoneDlg = DoneId;
	DoneFB = .15;
}

void APuzzleI06::PostLoad() {
	Super::PostLoad();
	// TODO 
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ06xC00"},
		{LDConsts::Items::Card1, "PZ06xC01"},
		{LDConsts::Items::Card2, "PZ06xC02"}
	};
	SetUseItemDlgs(Dlgs);

	static const TArray<bool> Locks = {false, false, false};
	SetLocks(Locks);
}

void APuzzleI06::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 4, 1};
	SetStates(States);

	UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;

	constexpr float DiffAm = .3;
	const float Diff = FMath::Lerp(-DiffAm, +DiffAm,
		Flags->Get(LDConsts::Flags::Settings::Global::Foxy));
	DoneFB += Diff;
	const FString& ClassName = StaticClass()->GetName();
	UE_LOG(LogTemp, Log, TEXT("%s::%hs foxify by=%.4f"), *ClassName, __func__, Diff);
}
