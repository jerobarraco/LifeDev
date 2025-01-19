// Copyright Jerónimo Barraco-Mármol

#include "LogicCard00.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Interact/Base/Range.h"

ULogicCard00::ULogicCard00():Super() {
	RangeClass = ARange::StaticClass();
}

void ULogicCard00::Use_Implementation() {
	if (UNLIKELY(!Range)) return;

	const AActor* const Pawn = UGameplayStatics::GetActorOfClass(this, ALChar::StaticClass());
	if (UNLIKELY(!Pawn)) return;
	
	const FVector& Location = Pawn->GetActorLocation();
	Range->SetActorLocation(Location);
	Range->TryTrigger();
	// UseDlg = "TV00_T"; // test
	Super::Use_Implementation();
}

void ULogicCard00::BeginPlay_Implementation(UWorld* const BrokenDontUse) {
	Super::BeginPlay_Implementation(BrokenDontUse);
	UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	Range = Cast<ARange>(W->SpawnActor(RangeClass, 0,0));
	if (UNLIKELY(!Range)) return;
}
