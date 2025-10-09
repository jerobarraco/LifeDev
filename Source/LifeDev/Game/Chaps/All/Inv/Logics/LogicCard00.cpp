// Copyright Jerónimo Barraco-Mármol

#include "LogicCard00.h"

#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Char/CLCharItems.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Interact/Base/Range.h"

// todo fix the case where you spawn this, but it has not compiled the material yet and it's visible with default mat.

ULogicCard00::ULogicCard00():Super() {
	RangeClass = ARange::StaticClass();
}

void ULogicCard00::Use_Implementation() {
	if (UNLIKELY(!ULSettings::GetFeatS(this, EFeat::G_CARD0))) return;
	if (UNLIKELY(!IsValid(Range))) return;

	const AActor* const Pawn = UGameplayStatics::GetActorOfClass(this,
		ALChar::StaticClass());
	if (UNLIKELY(!Pawn)) return;

	const FVector& Location = Pawn->GetActorLocation();
	Range->SetActorLocation(Location);
	Range->Trigger();

	Super::Use_Implementation();
}

void ULogicCard00::BeginPlay_Implementation() {
	Super::BeginPlay_Implementation();
	UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	Range = Cast<ARange>(W->SpawnActor(RangeClass, 0, 0));

	// duplicated code in card01 and 03
	UInventory* const Inv = UInventory::Instance(this);
	const UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags | !Inv)) return;

	FItem Item;
	if (UNLIKELY(!Inv->Get(LDConsts::Items::Card0, Item))) return;

	const float Foxy = Flags->Get(LDConsts::Flags::Settings::Global::Foxy);
	const float FoxyFact = (.75 + (.25*Foxy));
	const float NewCool = Item.CoolDown * FoxyFact; // goes from .75 to 1 in theory
	const bool Ok = Inv->SetCoolDown(LDConsts::Items::Card0, NewCool);

	Range->SetScaleMax(Range->DefScaleMax*FoxyFact);

	UE_LOG(LogTemp, Log, TEXT("LogicCard00::%hs Foxy=%.3f NewCool=%.3f Ok=%i"),
		__func__, Foxy, NewCool, Ok);
}

void ULogicCard00::BeginDestroy() {
	if (LIKELY(IsValid(Range))) Range->Destroy();
	Range = nullptr;

	Super::BeginDestroy();
}
