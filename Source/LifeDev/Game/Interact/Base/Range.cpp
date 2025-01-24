// Copyright Jerónimo Barraco-Mármol

#include "Range.h"

#include "Components/SphereComponent.h"
#include "Interact/Animator/AnimMat.h"

#include "JUtils/Actors/CQuickMesh.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Game/Interact/LInteract.h"
// this requires to create an object channel "Range"
// and edit the collision channel Interact to "overlap" range
// then add another collision profile for OverlapInteract that overlaps interacs and has type range.

ARange::ARange():Super() {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Super::SetActorTickEnabled(false); // ensure we don't animate on start

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Root);
	Collider->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	// Collider->SetCollisionProfileName("Interact");
	// Collider->SetCollisionProfileName("OverlapAllDynamic");
	Collider->SetCollisionProfileName("OverlapInteract");
	Collider->CanCharacterStepUpOn = ECB_No;
	Collider->InitSphereRadius(60); // 50 is ok but with 60 i make sure its bigger than the sphere
	
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);
	// Mesh->SetRelativeScale3D(FVector(UE_SMALL_NUMBER));
	Mesh->SetQuickCollisionEnabled(false);
	Mesh->bReceiveMobileCSMShadows = false;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (LIKELY(ObjMesh.Succeeded()))
		Mesh->SetStaticMesh(ObjMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		ObjMat(TEXT("/Game/LifeDev/Game/Inters/Cards/Card00Outline_MI.Card00Outline_MI"));
	if (LIKELY(ObjMat.Succeeded()))
		Mesh->SetMaterial(0, ObjMat.Object);

	Anim = CreateDefaultSubobject<UCAnimatorMix>(TEXT("Anim"));
	Anim->UseSweep = true;
	Anim->IsAdditive = false;
	Anim->Duration = 3;
	Anim->TStart.SetScale3D(FVector(.25)); // 1 would avoid flashing the player, but doesn't touch items that are close. (makes no sense)
	Anim->TEnd.SetScale3D(FVector(15));
	Anim->TRoot = Collider; // using the collider since sweep only happens for this component
	
	// using the mesh since the animator will mess with the location
	Anim->MatFName = TEXT("Opacity");
	Anim->MatFStart = 1;
	Anim->MatFEnd = 0;
}

void ARange::OverlapBegin(UPrimitiveComponent* const Cmp, AActor* const OtherActor,
	UPrimitiveComponent* const OtherComp, const int32 OtherBodyIndex,
	const bool bFromSweep, const FHitResult& SweepResult) {

	ALInteract* const Inter = Cast<ALInteract>(OtherActor);
	if (!Inter) return;

	UE_LOG(LogTemp, Log, TEXT("ARange::%hs got'em o=%s"), __func__, *GetNameSafe(OtherActor));
	Inter->ShowHint();
}

void ARange::BeginPlay() {
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	
	Anim->Mat = Mesh->CreateDynamicMaterialInstance(0);
	Anim->CodeCurve.Clear();
	Anim->Curve = nullptr;
	Anim->OnEnd.AddUniqueDynamic(this, &ARange::AnimEnd);
	// Anim->OnUpdate.AddUniqueDynamic(this, &ARange::AnimUpd); // nopes
	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARange::OverlapBegin);

	// UCodeCurveLib* const Lib = UCodeCurveLib::Instance();
	// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InSin);
}

void ARange::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Anim)) Anim->OnEnd.RemoveAll(this);
	if (LIKELY(Collider)) Collider->OnComponentBeginOverlap.RemoveAll(this);

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
