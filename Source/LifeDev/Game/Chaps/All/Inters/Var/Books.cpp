// Copyright Jerónimo Barraco-Mármol

#include "Books.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

ABooks::ABooks():Super() {
	// mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Books/BookP.BookP"));
	Mesh->SetRelativeLocation(FVector(-10,6.250000,0));

	// interact and sfx location
	const float ZLen = (Spacing*BookCount)/2;
	const FVector IntLocation(10,-6.250000, ZLen);
	Interact->SetRelativeLocation(IntLocation);
	Interact->SetBoxExtent(FVector(10, 6.25, ZLen));
	SFX->SetRelativeLocation(IntLocation);

	/// create 
	FRandomStream RS(RndFull ?  FMath::Rand()*MAX_int32 : RndSeed); // not static
	
	const int32 MatMax = Materials.Num() -1;
	for (int32 i =0; i<BookCount; ++i) {
		const FString SName = TEXT("Book_") + FString::FromInt(i);
		UCQuickMesh* const QM = CreateDefaultSubobject<UCQuickMesh>(FName(*SName));
		if (!QM) continue;

		QM->SetupAttachment(Mesh);
		QM->SetStaticMesh(CMesh.Object);
		const int32 OffY = RS.RandRange(-RndOff, RndOff);
		QM->SetRelativeLocation(FVector(0, OffY, Spacing*i));
		
		Books.Add(QM);
		AnimFade->Meshes.Add(QM);

		if (MatMax <= 0) continue;
		const int32 MatI = RS.RandRange(0, MatMax);

		TSoftObjectPtr<UMaterialInterface> Ptr = Materials[MatI];
		if (!Ptr.IsValid()) continue;
		
		UMaterialInterface* const Mat = Ptr.LoadSynchronous();
		if (!IsValid(Mat)) {
			UE_LOG(LogTemp, Warning, TEXT("%hs. could not get material ="), __func__, *Ptr.ToString());
			continue;
		}

		QM->SetMaterial(0, Mat);
	}

	/// end create
	// make them static for now
	UseAnim = false;
	SetEnabled(false);
	ABooks::SetMobility(EComponentMobility::Static);
}

void ABooks::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	for (UCQuickMesh* QM: Books) {
		QM->SetMobility(Mobility);
	}
}
 