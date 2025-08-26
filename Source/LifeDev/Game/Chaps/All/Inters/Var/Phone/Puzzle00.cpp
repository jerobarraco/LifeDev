// Copyright Jerónimo Barraco-Mármol

#include "Puzzle00.h"

#include "CQuickMesh.h"
#include "Interact/CPuzzle.h"

APuzzle00::APuzzle00():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {0,3,0,3,4,5,6};
	ResetTimeout = 3;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CM(TEXT("/Game/LifeDev/Game/Inters/Rooms/Phone/Phone00_P"));
	Mesh->SetStaticMesh(CM.Object);
	APuzzle00::SetMobility(EComponentMobility::Static);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Rooms/Phone/HangUp"));
	SFXs = {CSnd.Object, CSnd.Object};
}

void APuzzle00::BeginPlay() {
	Super::BeginPlay();
	// sets names in a simplified way
	TArray<TSoftObjectPtr<AInteract>> Inters = CPuzzle->GetPieces();
	const int32 Num = Inters.Num();
	for (int32 i= 0; UNLIKELY(i<Num); ++i) {
		AInteract* const I = Inters[i].Get();
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
