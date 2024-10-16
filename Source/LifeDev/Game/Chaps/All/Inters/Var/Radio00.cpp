// Copyright Jerónimo Barraco-Mármol

#include "Radio00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ARadio00::ARadio00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CDoor(TEXT("/Game/LifeDev/Game/Inters/Radio00/Radio00_Door.Radio00_Door"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CBase(TEXT("/Game/LifeDev/Game/Inters/Radio00/Radio00_Base.Radio00_Base"));
	Mesh->SetStaticMesh(CBase.Object);
	Mesh->SetRelativeLocation(FVector(0,30,0));
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(7.5,-30.,15));
	Interact->SetBoxExtent(FVector(7.5,30,15));

	DoorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRoot"));
	DoorRoot->SetupAttachment(IRoot);
	DoorRoot->SetRelativeLocation(FVector(15,0,10));
	// DoorRoot->SetRelativeRotation(FRotator(-35,0,0));

	Door = CreateDefaultSubobject<UCQuickMesh>(TEXT("Base"));
	Door->SetupAttachment(DoorRoot);
	Door->SetStaticMesh(CDoor.Object);
	Door->SetCastAllShadows(true);
	Door->SetRelativeLocation(FVector(-15,30,-10));

	SFX->SetRelativeLocation(FVector(12.5,0,12.5));

	Anim->TRoot = DoorRoot;
	Anim->TStart = Anim->TEnd = DoorRoot->GetRelativeTransform();
	// Anim->TStart.SetRotation(FRotator(-35,0., 0).Quaternion());
	// Anim->TStart.SetScale3D(FVector::OneVector);
	Anim->TEnd.SetRotation(FRotator(-35,0,0).Quaternion());
	// Anim->TEnd.SetScale3D(FVector::OneVector);
	Anim->IsAdditive = false;
	Anim->Duration = 1;
	// auto Lib = CreateDefaultSubobject<UCodeCurveLib>(TEXT("CodeCurve"));
	// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InCubic);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/InCubic_C.InCubic_C"));
	Anim->Curve = CCurve.Object;
	UseAnim = true;
	StateNum = 2;
	UseRewardDestroy = false;
	// AnimFade->SetNewMat();
	AnimFade->Meshes.Empty(); // don't fade this. it will also happily garble the material.
	AnimFade->SetAutoActivate(false);
	AnimFade->SetActive(false);

	Super::SetAutoActivate(true);
}

void ARadio00::BeginPlay() {
	Super::BeginPlay();
	SetState(1); // start opened
}
