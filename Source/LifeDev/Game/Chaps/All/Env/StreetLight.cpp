// Copyright Jerónimo Barraco-Mármol

#include "StreetLight.h"

#include "Components/SpotLightComponent.h"
#include "Interact/Animator/CAnimator.h"
#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Interact/CLSignificance.h"
#include "LifeDev/Game/Sys/Consts/ConstColors.h"

AStreetLight::AStreetLight():Super() {
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Spot = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot"));
	Spot->SetupAttachment(RootComponent);
	Spot->SetRelativeRotation(FRotator(-90,0,0));
	Spot->SetMobility(EComponentMobility::Stationary);
	Spot->SetIntensityUnits(ELightUnits::Lumens);
	Spot->SetIntensity(5);
	static const FColor Col(
		LDConsts::Colors::Palette[LDConsts::Colors::Brown][10]);
	// static constexpr FColor Col(207, 169, 134, 255);
	
	// (B=134,G=169,R=207,A=255)
	// (B=158,G=200,R=230,A=255)
	Spot->SetLightColor(Col, true);
	Spot->SetAttenuationRadius(500);
	Spot->SetInnerConeAngle(10);
	Spot->SetOuterConeAngle(40);

	Anim = CreateDefaultSubobject<UCAnimator>(TEXT("Anim"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/JUtils/Curves/Noise_C.Noise_C"));
	Anim->Curve = CCurve.Object;
	
	Randomizer = CreateDefaultSubobject<UCRandomizer>(TEXT("Randomizer"));
	Randomizer->IsLooping = true;
	Randomizer->UseRandReverse = true;
	Randomizer->Anim = Anim;
	Randomizer->DelayMin = 10;
	Randomizer->DelayMax = 50;
	Randomizer->ValueMin = 1;
	Randomizer->ValueMax = 5;
	Randomizer->SetAutoActivate(false); // important, it's feature flagged

	Sig = CreateDefaultSubobject<UCLSignificance>(TEXT("Sig"));
	// disabled since this requires a mesh. and that will add cost. and render check also adds a cost.
	Sig->OffscreenTimeMax = -1;
}


void AStreetLight::BeginPlay() {
	Super::BeginPlay();

	// don's strobe if it's not enabled
	if (!ULSettings::GetFeatS(GetWorld(), EFeat::A_STROBE)) return;
	Sig->BindAnim(Anim);
	Anim->OnUpdate.AddUniqueDynamic(this, &AStreetLight::AnimUpdate);
}

void AStreetLight::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(Sig)) {
		Sig->UnbindAnim();
	}
	Sig = nullptr;

	if (IsValid(Anim)) {
		Anim->OnUpdate.RemoveAll(this);
	}
	Anim = nullptr;

	if (IsValid(Randomizer)) {
		Randomizer->Deactivate();
	}
	Randomizer = nullptr;
	Super::EndPlay(EndPlayReason);
}

void AStreetLight::AnimUpdate(float Progress, float Alpha) {
	Spot->SetIntensity(IntensityMax*Alpha);
}
