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

static const TCHAR* _mats[] = {
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI.Palette00_DMI"),
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette01_DMI.Palette01_DMI"),
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette02_DMI.Palette02_DMI"),
	TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette03_DMI.Palette03_DMI"),
};

void ABooks::CreateBooks() {
	/// create
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Books/BookP.BookP"));
	// check against 0 to be able to use negative values as well
	const FRandomStream RS(RndSeed == 0 ? FMath::Rand() : RndSeed); // not static

	const int32 MatMax = UJUtilsMisc::ArraySize(_mats) -1;
	for (int32 i =0; i<BookCount; ++i) {
		const FString SName = TEXT("Book_") + FString::FromInt(i);
		UCQuickMesh* const QM = CreateDefaultSubobject<UCQuickMesh>(FName(*SName));
		if (UNLIKELY(!QM)) continue;

		QM->SetupAttachment(Mesh);
		QM->SetStaticMesh(CMesh.Object);
		const int32 OffY = RS.RandRange(-RndOff, RndOff);
		QM->SetRelativeLocation(FVector(0, OffY, Spacing*i));
		QM->SetCastAllShadows(true);

		Books.Add(QM);
		AnimFade->Meshes.Add(QM);

		// since this is static, rider will hint this out, but it's good to keep
		if (UNLIKELY(MatMax <= 0)) continue;

		const int32 MatI = RS.RandRange(0, MatMax);

		ConstructorHelpers::FObjectFinder<UMaterialInstance> M(_mats[MatI]);
		if (UNLIKELY(!IsValid(M.Object))) continue;

		UMaterialInterface* const Mat = M.Object;
		if (UNLIKELY(!IsValid(Mat))) {
			UE_LOG(LogTemp, Warning, TEXT("%hs. Could not get material. matI=%i"),
				__func__, MatI);
			continue;
		}

		QM->SetMaterial(0, Mat);
	}
}

void ABooks::Constructor() {
	UseAnim = false;
	UseRewardDestroy = false;
	StateNum = 1;
	// UseAutoActivate = false; // important since i use these as decoration a lot // redundant
	Texts = { NSLOCTEXT("Books", "State0", "Books")};
	// mesh (what's this for again?)
	Mesh->SetRelativeLocation(FVector(-10, 6.25, 0));

	CreateBooks();
	SetUpInteract();

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C"));
	// SFXTrigger = CSnd.Object;
	SFXs = { CSnd.Object };

	/// end create
	// make them static for now

	SetMobility(EComponentMobility::Static);
}

// unreal made me do it.

ABooks::ABooks():Super() {
	Constructor(); // call the child constructor
}

ABooks::ABooks(const int32 nBookCount, const int32 nRndSeed):Super() {
	// check to avoid issues 
	if (nBookCount>=0) BookCount = nBookCount;
	RndSeed = nRndSeed;

	Constructor(); // call the child constructor
}

void ABooks::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	for (const TObjectPtr<UCQuickMesh>& QM: Books) {
		if (UNLIKELY(!QM)) continue;
		QM->SetMobility(Mobility);
	}
}
