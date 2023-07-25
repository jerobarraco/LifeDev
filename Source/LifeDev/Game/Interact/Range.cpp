// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

#include "Interact/Animator/CAnimatorMix.h"

ARange::ARange():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false); // ensure we don't animate on start

	IRoot->SetRelativeScale3D(FVector(0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ObjMat(TEXT("/Game/LifeDev/Game/Var/Mats/Outline/OutlineDepth_MI.OutlineDepth_MI"));
	if (ObjMat.Succeeded()) {
		Mat = ObjMat.Object;
	}

	Anim->IsAdditive = false;
	Anim->Duration = 3;
	Anim->TEnd.SetScale3D(FVector(10));
	Anim->MatFName = TEXT("Opacity");
	Anim->MatFStart = 1;
	Anim->MatFEnd = 0;
}

void ARange::BeginPlay() {
	Super::BeginPlay();
	Anim->Mat = Mesh->CreateDynamicMaterialInstance(0, Mat); 
}

void ARange::Trigger_Implementation() {
	IRoot->SetRelativeScale3D(FVector(0));
	Super::Trigger_Implementation(); // TODO should i call it or not?
}

void ARange::SetMaxSize(float Size) {
	Anim->TEnd.SetScale3D(FVector(Size));
}