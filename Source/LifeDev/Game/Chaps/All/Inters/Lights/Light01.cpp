// Copyright Jerónimo Barraco-Mármol

#include "Light01.h"

#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight01::ALight01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Light00/Light00-Frame.Light00-Frame"));
	Mesh->SetRelativeLocation(FVector(-27.500000,27.500000,0.000000));
	Mesh->SetStaticMesh(CMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Light00/Light00-GlassB.Light00-GlassB"));
	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tube"));
	Glass->SetupAttachment(Mesh);
	Glass->SetStaticMesh(CTube.Object);
	Glass->SetRelativeLocation(FVector(5.,-5.,5.));
	Glass->SetCastAllShadows(false);

	/// anim
	// enable the animation so that we get the update. but don't actually animate any material
	UseAnim = true;
	Anim->Mat = nullptr;
	// Anim->MatVEnd = FLinearColor(1, 1, 1, 1);
	// Anim->MatVName = "Emissive";
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/JUtils/Curves/NoiseRamp_C.NoiseRamp_C"));
	if (CCurve.Succeeded()) {
		Anim->Curve = CCurve.Object;
	}
	
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Mesh);
	Light->SetRelativeLocation(FVector(27.5,-27.5,9));
	Light->SetIntensityUnits(ELightUnits::Lumens);
	Light->SetIntensity(IntensityMax);
	Light->SetAttenuationRadius(700);
	Light->SetSourceRadius(10);
	
	Interact->SetRelativeLocation(FVector(27.500000,-27.500000,15));
	Interact->SetBoxExtent(FVector(28,28.5,15.5));

	ALight01::SetMobility(EComponentMobility::Static);
}

void ALight01::UpdateAnim(float Progress, float Alpha) {
	Light->SetIntensity(IntensityMax*Alpha);
	const bool IsOn = Alpha >= .05;
	Light->SetVisibility(IsOn); // optimization
}

void ALight01::BeginPlay() {
	Super::BeginPlay();	
	Anim->OnUpdate.AddUniqueDynamic(this, &ALight01::UpdateAnim);
}

void ALight01::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Anim->OnUpdate.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void ALight01::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Glass->SetMobility(Mobility);
	Light->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}

