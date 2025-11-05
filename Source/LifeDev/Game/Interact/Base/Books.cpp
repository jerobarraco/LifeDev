// Copyright Jerónimo Barraco-Mármol

#include "Books.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Misc/JUtilsMisc.h"

void ABooks::SetUpInteract() const {
	// interact and sfx location
	const float ZLen = (Spacing*BookCount)/2.0;
	const FVector IntLocation(10,-6.250000, ZLen);
	Interact->SetRelativeLocation(IntLocation);
	Interact->SetBoxExtent(FVector(10, 6.25 + RndOff, ZLen));
	SFX->SetRelativeLocation(IntLocation);
}

static const TCHAR* _matsB[] = {
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI.Palette00_DMI"),
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette01_DMI.Palette01_DMI"),
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette02_DMI.Palette02_DMI"),
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette03_DMI.Palette03_DMI"),
};

ABooks::ABooks():Super() {
	UseAnim = false;
	UseRewardDestroy = false;
	StateNum = 1;
	// UseAutoActivate = false; // important since i use these as decoration a lot // redundant
	Texts = { NSLOCTEXT("Books", "State0", "Books")};
	// mesh (what's this for again?)
	Mesh->SetRelativeLocation(FVector(-10, 6.25, 0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Books/BookP.BookP"));
	BaseMesh = CMesh.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C"));
	// SFXTrigger = CSnd.Object;
	SFXs = { CSnd.Object };

	const uint8 MatMax = UJUtilsMisc::ArraySize(_matsB);
	for (uint8 i = 0; i<MatMax; ++i) {
		ConstructorHelpers::FObjectFinder<UMaterialInterface> M(_matsB[i]);
		UMaterialInterface* const MM = M.Object;
		if (UNLIKELY(!IsValid(MM))) continue;

		Materials.Add(MM);
	}
	/// end create
	// make them static for now

	ABooks::SetMobility(EComponentMobility::Static);
}

void ABooks::ReCreate() {
	DestroyBooks();
	CreateBooks();
	SetUpInteract(); // depends on the number of books
}

void ABooks::DestroyBooks() {
	for (UCQuickMesh* const C: Books) {
		if (UNLIKELY(!IsValid(C))) continue;

		RemoveOwnedComponent(C);
		C->DestroyComponent(false);
	}

	Books.Empty();
	AnimFade->Meshes.Empty();
}

void ABooks::CreateBooks() {
	/// create

	// check against 0 to be able to use negative values as well
	RndSeedGen = RndSeed == 0 ? FMath::Rand() : RndSeed; // this is a test. move to recreate and replace rndseed
	const FRandomStream RS(RndSeedGen); // not static

	// https://forums.unrealengine.com/t/what-is-the-correct-way-to-create-and-add-components-at-runtime/15605/21?u=nande
	const int32 MatMax = Materials.Num() -1;
	for (int32 i =0; i<BookCount; ++i) {
		const FString SName = TEXT("Book_") + FString::FromInt(i);
		UCQuickMesh* const QM = Cast<UCQuickMesh>(
			AddComponentByClass(UCQuickMesh::StaticClass(), true, FTransform::Identity, false));

		if(UNLIKELY(!QM)) {
			UE_LOG(LogTemp, Warning, TEXT("%hs could not create components"), __func__);
			return;
		}
		
		AddOwnedComponent(QM);
		QM->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
		QM->SetStaticMesh(BaseMesh);
		const int32 OffY = RS.RandRange(-RndOff, RndOff);
		QM->SetRelativeLocation(FVector(0, OffY, Spacing*i));
		QM->SetUseDynShadow(true);

		Books.Add(QM);
		AnimFade->Meshes.Add(QM);

		if (UNLIKELY(MatMax <= 0)) continue;

		const int32 MatI = RS.RandRange(0, MatMax);
		
		UMaterialInterface* const Mat = Materials[MatI];
		if (UNLIKELY(!IsValid(Mat))) {
			UE_LOG(LogTemp, Warning, TEXT("%hs. Could not get material. matI=%i"),
				__func__, MatI);
			continue;
		}

		QM->SetMaterial(0, Mat);
	}
}

void ABooks::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	for (const TObjectPtr<UCQuickMesh>& QM: Books) {
		if (UNLIKELY(!QM)) continue;
		QM->SetMobility(Mobility);
	}
}

void ABooks::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	// https://forums.unrealengine.com/t/added-components-not-showing-in-details-panel/465572/3?u=nande
	// this is actually the correct place. it has the side effect that it will recreate when a property is changed.
	ReCreate();
	// https://forums.unrealengine.com/t/components-added-at-runtime-dont-show-up-in-editor-details-panel/155064/5?u=nande
}
