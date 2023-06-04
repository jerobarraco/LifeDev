// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

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
	

	Animator = CreateDefaultSubobject<UCAnimatorTrans>(TEXT("Animator"));
	Animator->AnimRoot = AnimRoot;
	Animator->IsAdditive = false;
	Animator->Duration = 3;
}

void ARange::BeginPlay() {
	Super::BeginPlay();
	Animator->OnUpdate.AddUniqueDynamic(this, &ARange::AnimUpdate);
	MatI = Mesh->CreateDynamicMaterialInstance(0, Mat);
}

void ARange::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Animator->OnUpdate.RemoveAll(this);
}

void ARange::Trigger_Implementation() {
	AnimRoot->SetRelativeScale3D(FVector(0));
	Animator->Play();
}

void ARange::SetMaxSize(float Size) {
	Animator->TEnd.SetScale3D(FVector(Size));
}

void ARange::AnimUpdate(float Progress, float Alpha) {
	MatI->SetScalarParameterValue(TEXT("Opacity"), 1-Alpha);
}
