// Copyright Jerónimo Barraco-Mármol

#include "LogicCard00.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Interact/Base/Range.h"

ULogicCard00::ULogicCard00():Super() {
	RangeClass = ARange::StaticClass();
}

void ULogicCard00::Use_Implementation() {
	const AActor* const Pawn = UGameplayStatics::GetActorOfClass(this, ALChar::StaticClass());
	if (UNLIKELY(!Pawn)) return;
	
	const FVector& Location = Pawn->GetActorLocation();
	const FRotator Rotation(0);
	ARange* const Range = Cast<ARange>(GetWorld()->SpawnActor(RangeClass, &Location, &Rotation));
	if (UNLIKELY(!Range)) return;
	Range->TryTrigger();
	// UseDlg = "TV00_T"; // test
	Super::Use_Implementation();
}
