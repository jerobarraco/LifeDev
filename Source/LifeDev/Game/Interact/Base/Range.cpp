// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ARange::ARange():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Super::SetActorTickEnabled(false); // ensure we don't animate on start

	IRoot->SetRelativeScale3D(FVector(SMALL_NUMBER));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (LIKELY(ObjMesh.Succeeded()))
		Mesh->SetStaticMesh(ObjMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		ObjMat(TEXT("/Game/LifeDev/Game/Inters/Cards/Card00Outline_MI.Card00Outline_MI"));
	if (LIKELY(ObjMat.Succeeded()))
		Mesh->SetMaterial(0, ObjMat.Object);// Mat = ObjMat.Object;

	Mesh->bReceiveMobileCSMShadows = false;
	StateNum = 1;
	Texts = {FText::GetEmpty()};
	Anim->IsAdditive = false;
	Anim->Duration = 3;
	Anim->TStart.SetScale3D(FVector(1));
	Anim->TEnd.SetScale3D(FVector(15));
	Anim->MatFName = TEXT("Opacity");
	Anim->MatFStart = 1;
	Anim->MatFEnd = 0;
	DisableWhileAnim = false; // avoid getting reactivated
	IsOneShot = false;
	SetAutoActivate(false);
}

void ARange::BeginPlay() {
	Super::BeginPlay();
	// SetActorHiddenInGame(true);
	
	Anim->Mat = Mesh->CreateDynamicMaterialInstance(0);
	Anim->CodeCurve.Clear();
	Anim->Curve = nullptr;
	// UCodeCurveLib* const Lib = UCodeCurveLib::Instance();
	// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InSin);
}

void ARange::AnimEnd_Implementation() {
	Super::AnimEnd_Implementation();
	// SetActorHiddenInGame(true);
}

bool ARange::TryTrigger_Implementation() {
	Anim->Deactivate(); // force the animation to stop so that it triggers again.
	return Super::TryTrigger_Implementation();
}

void ARange::DoTrigger_Implementation() {
	// IRoot->SetRelativeScale3D(FVector(0));
	// SetActorHiddenInGame(false);
	Super::DoTrigger_Implementation();
}

void ARange::SetMaxSize(const float Size) {
	Anim->TEnd.SetScale3D(FVector(Size));
}

// disable the hidden
// use the new material
// /Script/Engine.MaterialInstanceConstant'/Game/LifeDev/Game/Inters/Cards/Card00Outline_MI.Card00Outline_MI'