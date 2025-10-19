// Copyright Jerónimo Barraco-Mármol

#include "LogicCard00.h"

#include "Kismet/GameplayStatics.h"

#include "Inventory/Flags.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Settings/LSettings.h"
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

	if (UNLIKELY(!Flags)) return;

	const float Foxy = Flags->Get(LDConsts::Flags::Settings::Global::Foxy);
	const float FoxyFact = (.75 + (.25*Foxy));
	Range->SetScaleMax(Range->DefScaleMax*FoxyFact);

	UE_LOG(LogTemp, Log, TEXT("LogicCard00::%hs Foxy=%.3f Fact=%.3f"),
		__func__, Foxy, FoxyFact);
}

void ULogicCard00::BeginDestroy() {
	if (LIKELY(IsValid(Range))) Range->Destroy();
	Range = nullptr;

	Super::BeginDestroy();
}
