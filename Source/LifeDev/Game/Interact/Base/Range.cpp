// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ARange::ARange():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Super::SetActorTickEnabled(false); // ensure we don't animate on start

	IRoot->SetRelativeScale3D(FVector(0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (LIKELY(ObjMesh.Succeeded())) Mesh->SetStaticMesh(ObjMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		ObjMat(TEXT("/JUtils/Mats/Outline/OutlineDepth_MI.OutlineDepth_MI"));
	if (LIKELY(ObjMat.Succeeded())) Mat = ObjMat.Object;

	Anim->IsAdditive = false;
	Anim->Duration = 5;
	Anim->TEnd.SetScale3D(FVector(100));
	Anim->MatFName = TEXT("Opacity");
	Anim->MatFStart = 1;
	Anim->MatFEnd = 0;
}

void ARange::BeginPlay() {
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	
	Anim->Mat = Mesh->CreateDynamicMaterialInstance(0, Mat);
	UCodeCurveLib* const Lib = UCodeCurveLib::Instance();
	Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::OutCubic);
}

void ARange::AnimEnd_Implementation() {
	Super::AnimEnd_Implementation();
	SetActorHiddenInGame(true);
}

void ARange::DoTrigger_Implementation() {
	IRoot->SetRelativeScale3D(FVector(0));
	SetActorHiddenInGame(false);
	Super::DoTrigger_Implementation();
}

void ARange::SetMaxSize(const float Size) {
	Anim->TEnd.SetScale3D(FVector(Size));
}