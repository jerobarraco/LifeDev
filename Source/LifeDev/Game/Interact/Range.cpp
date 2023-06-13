// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

#include "Interact/CAnimatorMix.h"
#include "Interact/CAnimatorTrans.h"

ARange::ARange():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false); // ensure we don't animate on start
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	AnimRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AnimRoot"));
	AnimRoot->SetupAttachment(RootComponent);
	AnimRoot->SetRelativeScale3D(FVector(0));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(AnimRoot);
	Mesh->PrimaryComponentTick.bStartWithTickEnabled = false;
	Mesh->SetComponentTickEnabled(false);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh->SetCanEverAffectNavigation(false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ObjMat(TEXT("/Game/LifeDev/Game/Chaps/All/Mats/MI_OutlineDepth.MI_OutlineDepth"));
	if (ObjMat.Succeeded()) {
		Mat = ObjMat.Object;
	}
	Anim->TRoot = AnimRoot;
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
	AnimRoot->SetRelativeScale3D(FVector(0));
	Super::Trigger_Implementation(); // TODO should i call it or not?
}

void ARange::SetMaxSize(float Size) {
	Anim->TEnd.SetScale3D(FVector(Size));
}