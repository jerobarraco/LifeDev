// Copyright Jerónimo Barraco-Mármol

#include "LogicCard02.h"

#include "Inventory/Flags.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ULogicCard02::ULogicCard02() {}

void ULogicCard02::Use_Implementation() {
	if (UNLIKELY(!ULSettings::GetFeatS(this, EFeat::G_CARD2))) return;

	Super::Use_Implementation();
	if (UNLIKELY(!FB)) return;

	FB->ModVal(FMath::FRandRange(ModMin, ModMax)); // lowers the fb by a random
}

void ULogicCard02::BeginPlay_Implementation() {
	Super::BeginPlay_Implementation();

	// get the max from the default class, in case this begin play gets executed multiple times.
	const ULogicCard02* const Default = GetMutableDefault<ULogicCard02>();
	const float ModMaxDef = LIKELY(Default) ? Default->ModMax : ModMax;
	const float Foxify = Flags->Get(LDConsts::Flags::Settings::Global::Foxy);
	const float HalfDiff = (ModMax - ModMin)/2.0;
	const float ModMid = ModMin + HalfDiff;
	const float ModNew = FMath::LerpStable(ModMaxDef, ModMid, Foxify);
	UE_LOG(LogTemp, Log, TEXT("LogicCard02::%hs Foxify=%.3f HDiff=%.3f Mid=%.3f Max=%.3f New=%.3f"),
		__func__, Foxify, HalfDiff, ModMid, ModMax, ModNew);
	ModMax = ModMid;
}
