// Copyright Jerónimo Barraco-Mármol

#include "LogicCard00.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Interact/Base/Range.h"

// TODO have a collision (maybe the interact itself? it has to be a sphere)
// make other interacts react to the collisions and write to a custom channel
// needs an anim to animate down
// do i need a special colission channel? or can i just check the type?
// maybe i can just add the logic to this class?
// i would need multiple animators :/
ULogicCard00::ULogicCard00():Super() {
	RangeClass = ARange::StaticClass();
}

void ULogicCard00::Use_Implementation() {
	const AActor* const Pawn = UGameplayStatics::GetActorOfClass(this, ALChar::StaticClass());
	if (UNLIKELY(!Pawn)) return;
	
	UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	const FVector& Location = Pawn->GetActorLocation();

	// have to spawn on use since beginplay gets executed way too early.
	if (UNLIKELY(!IsValid(Range)))
		Range = Cast<ARange>(W->SpawnActor(RangeClass, 0,0));
	Range->SetActorLocation(Location);
	Range->TryTrigger();
	// UseDlg = "TV00_T"; // test
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
