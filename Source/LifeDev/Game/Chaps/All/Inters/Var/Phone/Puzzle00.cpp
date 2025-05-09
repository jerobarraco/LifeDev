// Copyright Jerónimo Barraco-Mármol

#include "Puzzle00.h"

#include "CQuickMesh.h"
#include "Interact/CPuzzle.h"

APuzzle00::APuzzle00():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {0,3,0,3,4,5,6};

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CM(TEXT("/Game/LifeDev/Game/Inters/Var/Phone00_P"));
	Mesh->SetStaticMesh(CM.Object);
	SetMobility(EComponentMobility::Static);
	ResetTimeout = 3;
}

void APuzzle00::PostLoad() {
	// by now the interacts set in editor are loaded
	Super::PostLoad();
	// sets names in a simplified way
	TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	const int32 Num = Inters.Num();
	for (int32 i= 0; UNLIKELY(i<Num); ++i) {
		AInteract* const I = Inters[i];
		if (UNLIKELY(!I)) continue;

		const FString& SI =
			i < 10 ? FString::FromInt(i) :
			(i < 11 ? TEXT("*"): TEXT("#"));
		const FText& TI = FText::FromString(SI);
		I->Texts = { TI, TI };
	}
	// static const TArray<bool> Locks = { true, false, true, false };
	
	// SetLocks(Locks);
}
//
// void APuzzle00::BeginPlay() {
// 	Super::BeginPlay();
// 	static const TArray<int32> States = { 5,0,0,0 };
// 	SetStates(States);
// }
