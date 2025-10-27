// Copyright Jerónimo Barraco-Mármol

#include "Light00.h"

#include "Components/RectLightComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight00::ALight00() {
	UseFade = false;
	UseRewardDestroy = false;

	/// anim
	UseAnim = true;
	Anim->MatVStart = FLinearColor::Black;
	Anim->MatVEnd = FLinearColor::White;
	Anim->MatVName = "Emissive";
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/C_Fluorescent.C_Fluorescent"));
	if (LIKELY(CCurve.Succeeded())) Anim->Curve = CCurve.Object;

	// objects

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/Support"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetRelativeRotation(FRotator(0,270,90));
	Mesh->SetRelativeScale3D(FVector(1,1,1));
	
	Interact->SetRelativeLocation(FVector(2.500000,-2.500000,0));
	Interact->SetBoxExtent(FVector(42.500000,7.500000,5.000000));

	RectLight->SetRelativeLocation(FVector(2.500000,-2.500000,-0.200000));
	RectLight->SetRelativeRotation(FRotator(-90,-90,0));
	RectLight->SetSourceWidth(75);
	RectLight->SetSourceHeight(5);
	RectLight->SetBarnDoorAngle(90.000000);
	RectLight->SetBarnDoorLength(7);
	RectLight->SetIntensity(700);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/Fluorescent"));
	// the default material blocks the light, so i decided to use this one.
	// it's set on the mesh itself, so no need to load it here.
	// also using the D version. this mat has emissive as well.
	// static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		// CTubeMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/VoxelGlassSemi_DMI"));
	Tube = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tube"));
	Tube->SetupAttachment(Mesh);
	if (LIKELY(CTube.Succeeded())) Tube->SetStaticMesh(CTube.Object);
	
	Tube->SetRelativeLocation(FVector(0,0,-5));
	Tube->SetCastAllShadows(false);

	ALight00::SetMobility(EComponentMobility::Static);
}

void ALight00::BeginPlay() {
	Super::BeginPlay();
	// DO NOT CREATE material instance on the constructor. or it will crash the editor at best.
	// using metal instead of glass to avoid having to deal with transparency.
	// as long as it emits when it's on. it might not be an issue. fluorescents are not transparent.
	// passing nullptr uses the current material. note this could return nullptr, in which case the animmat will skip.
	Anim->Mat = Tube->CreateDynamicMaterialInstance(0, nullptr);
}

void ALight00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Anim->Mat = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALight00::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Tube->SetMobility(Mobility);
}

