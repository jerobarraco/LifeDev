// Copyright Jerónimo Barraco-Mármol

#include "LogicCard00.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Interact/Base/Range.h"

// todo fix the case where you spawn this, but it has not compiled the material yet.

ULogicCard00::ULogicCard00():Super() {
	RangeClass = ARange::StaticClass();
}

void ULogicCard00::Use_Implementation() {
	if (UNLIKELY(!ULSettings::GetFeatS(this, EFeat::G_CARD0))) return;

	const AActor* const Pawn = UGameplayStatics::GetActorOfClass(this, ALChar::StaticClass());
	if (UNLIKELY(!Pawn)) return;
	
	UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	const FVector& Location = Pawn->GetActorLocation();

	// have to spawn on use since beginplay gets executed way too early.
	if (UNLIKELY(!IsValid(Range)))
		Range = Cast<ARange>(W->SpawnActor(RangeClass, 0,0));
	Range->SetActorLocation(Location);
	Range->Trigger();

	Super::Use_Implementation();
}

void ULogicCard00::BeginPlay_Implementation() {
	Super::BeginPlay_Implementation();
}

void ULogicCard00::BeginDestroy() {
	if (LIKELY(IsValid(Range))) Range->Destroy();
	Range = nullptr;

	Super::BeginDestroy();
}
