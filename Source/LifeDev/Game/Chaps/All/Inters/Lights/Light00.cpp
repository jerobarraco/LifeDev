// Copyright Jerónimo Barraco-Mármol

#include "Light00.h"

#include "Components/RectLightComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"
#include "JUtils/Misc/JMiscConsts.h"

ALight00::ALight00():Super() {
	UseFade = false;
	UseRewardDestroy = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/Support.Support"));
	Mesh->SetRelativeLocation(FVector(-2.5,2.5,0));
	Mesh->SetRelativeScale3D(FVector(0.05,0.05,0.05));
	Mesh->SetStaticMesh(CMesh.Object);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/Fluorescent.Fluorescent"));
	Tube = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tube"));
	Tube->SetupAttachment(Mesh);
	IFL(CTube.Succeeded()) Tube->SetStaticMesh(CTube.Object);

	Tube->SetRelativeLocation(FVector(0.5,7.5,100));
	Tube->SetCastAllShadows(false);

	/// anim
	UseAnim = true;
	Anim->MatVStart = FLinearColor::Black;
	Anim->MatVEnd = FLinearColor::White;
	Anim->MatVName = "Emissive";
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/C_Fluorescent.C_Fluorescent"));
	IFL (CCurve.Succeeded()) Anim->Curve = CCurve.Object;
	
	RectLight->SetRelativeLocation(FVector(0.5,7.5,100));
	RectLight->SetRelativeRotation(FRotator(90,90,0));
	RectLight->SetSourceWidth(75);
	RectLight->SetSourceHeight(5);
	RectLight->SetBarnDoorAngle(90.000000);
	RectLight->SetBarnDoorLength(7);

	Interact->SetRelativeLocation(FVector(40,-50,103.734790));
	Interact->SetBoxExtent(FVector(900,200,150));

	ALight00::SetMobility(EComponentMobility::Static);
}

void ALight00::BeginPlay() {
	Super::BeginPlay();
	// TODO move the material loading to the CDO. see TV00
	// DO NOT CREATE material instance on the constructor. or it will crash the editor at best.
	// using metal instead of glass to avoid having to deal with transparency.
	// as long as it emits when it's on. it might not be an issue. fluorescents are not transparent.
	// passing nullptr uses the current material. note this could return nullptr, in which case the animmat will skip.
	UMaterialInstanceDynamic* const MI = Tube->CreateDynamicMaterialInstance(0, nullptr);
	Anim->Mat = MI;
}

void ALight00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Anim->Mat = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALight00::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Tube->SetMobility(Mobility);
}

