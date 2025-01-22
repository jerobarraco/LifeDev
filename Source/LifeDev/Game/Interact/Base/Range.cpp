// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

#include "Components/SphereComponent.h"

#include "JUtils/Actors/CQuickMesh.h"

#include "Interact/Animator/CAnimatorMix.h"

ARange::ARange():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Super::SetActorTickEnabled(false); // ensure we don't animate on start

	USceneComponent* const Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeScale3D(FVector(UE_SMALL_NUMBER));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (LIKELY(ObjMesh.Succeeded()))
		Mesh->SetStaticMesh(ObjMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		ObjMat(TEXT("/Game/LifeDev/Game/Inters/Cards/Card00Outline_MI.Card00Outline_MI"));
	if (LIKELY(ObjMat.Succeeded()))
		Mesh->SetMaterial(0, ObjMat.Object);

	Mesh->bReceiveMobileCSMShadows = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Mesh);

	Anim = CreateDefaultSubobject<UCAnimatorMix>(TEXT("Anim"));
	Anim->IsAdditive = false;
	Anim->Duration = 3;
	Anim->TStart.SetScale3D(FVector(1)); // this scale avoids flashing the player
	Anim->TEnd.SetScale3D(FVector(15));
	Anim->TRoot = Mesh; // using the mesh since the animator will mess with the location
	Anim->MatFName = TEXT("Opacity");
	Anim->MatFStart = 1;
	Anim->MatFEnd = 0;
}

void ARange::BeginPlay() {
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	
	Anim->Mat = Mesh->CreateDynamicMaterialInstance(0);
	Anim->CodeCurve.Clear();
	Anim->Curve = nullptr;
	Anim->OnEnd.AddUniqueDynamic(this, &ARange::AnimEnd);
	// UCodeCurveLib* const Lib = UCodeCurveLib::Instance();
	// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InSin);
}

void ARange::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Anim)) Anim->OnEnd.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void ARange::AnimEnd() {
	SetActorHiddenInGame(true);
}

void ARange::Trigger() {
	SetActorHiddenInGame(false);
	Anim->Activate(true); // force the animation to stop so that it triggers again.
}

void ARange::SetMaxScale(const float Scale) const {
	Anim->TEnd.SetScale3D(FVector(Scale));
}
