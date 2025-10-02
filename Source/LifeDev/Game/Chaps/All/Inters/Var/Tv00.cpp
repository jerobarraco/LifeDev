// Copyright Jerónimo Barraco-Mármol

#include "Tv00.h"

#include "Components/AudioComponent.h"

#include "JSig/CSignificance.h"
#include "JUtils/Actors/CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Interact/CLSignificance.h"
#include "LifeDev/Core/Sounds/CLSounder.h"

ATv00::ATv00():Super() {
	// can't set meshes to static or the button animation won't work :'(
	// so much optimization lost for a single button animation...
	Texts = {
		NSLOCTEXT("TV00", "State0", "Turn On"),
		NSLOCTEXT("TV00", "State0", "Turn Off") };
	UseAutoActivate = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Tv00/Tv00-Btn.Tv00-Btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-32.5,27.5,0));

	// the extent is overflowing towards the front so that when the mesh animates
	// back, it will still be easily triggerable
	Interact->SetRelativeLocation(FVector(32.5,-22.5,25));
	Interact->SetBoxExtent(FVector(35,35,25));

	// btn position from relavite
	SFX->SetRelativeLocation(FVector(25.000000,25.000000,-10.000000));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Inters/Generic/Button_Press-007.Button_Press-007"));
	SFXs = {SOpen.Object, SOpen.Object}; // reusing the same. close, open
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpenEnd(TEXT("/Game/LifeDev/Game/Inters/Tv00/OldRadioTuningStaticNoise_001_Cue"));
	SFX_Stop = {nullptr, SOpenEnd.Object};
	
	/// other meshes
	Frame = CreateDefaultSubobject<UCQuickMesh>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CFrame(TEXT("/Game/LifeDev/Game/Inters/Tv00/Tv00-Base.Tv00-Base"));
	Frame->SetStaticMesh(CFrame.Object);
	Frame->SetRelativeLocation(FVector(-32.5,27.5,0));
	Frame->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Frame->SetCollisionProfileName("BlockAllDynamic");
	Frame->SetCastAllShadows(true);
	
	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Glass"));
	Glass->SetupAttachment(Frame);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CGlass(TEXT("/Game/LifeDev/Game/Inters/Tv00/Tv00-Screen.Tv00-Screen"));
	Glass->SetStaticMesh(CGlass.Object);
	Glass->SetCastAllShadows(false); // opt
	Glass->bUseAttachParentBound = true; // opt
	
	Crt = CreateDefaultSubobject<UCQuickMesh>(TEXT("Crt"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCrt(TEXT("/Game/LifeDev/Game/Inters/Tv00/Tv00-Crt.Tv00-Crt"));
	Crt->SetupAttachment(Frame);
	Crt->SetStaticMesh(CCrt.Object);
	Crt->SetCastAllShadows(false); // opt
	Crt->bUseAttachParentBound = true; // opt
	
	/// anims
	UseAnim = true;
	// button anim
	Anim->Duration = .5;
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector(0.0,-2.5,0));
	AnimFade->Meshes.Empty();

	// crt anim
	AnimCrt = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimCrt"));
	AnimCrt->MatVName = "Emissive";
	AnimCrt->MatVStart = FLinearColor::Black;
	AnimCrt->MatVEnd = FLinearColor(5, 5, 5, 1);
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurveMat(TEXT("/JUtils/Curves/NoiseRamp_C.NoiseRamp_C"));
	AnimCrt->Curve = CCurveMat.Object;
	AnimCrt->Duration = 2; // initial duration
	AnimCrt->SetComponentTickInterval(1/30); // opt. light anim does not need to be 60.
	
	// randomizer for the anim
	RndCrt = CreateDefaultSubobject<UCRandomizer>(TEXT("RndCrt"));
	RndCrt->Anim = AnimCrt;
	RndCrt->DelayMin = .5;
	RndCrt->DelayMax = 3;
	RndCrt->ValueMin = .5;
	RndCrt->ValueMax = 3;
	RndCrt->IsLooping = true;
	RndCrt->UseAnimRandReverse = true;

	Noise = CreateDefaultSubobject<UCLSounder>(TEXT("Noise"));
	Noise->SetupAttachment(IRoot);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSNoise(TEXT("/Game/LifeDev/Game/Inters/Tv00/NOISE_EMF_Radiation__Constant_Hum__LCD_TV_Screen_Off.NOISE_EMF_Radiation__Constant_Hum__LCD_TV_Screen_Off"));
	Noise->SetSound(CSNoise.Object);

	Sig = CreateDefaultSubobject<UCLSignificance>(TEXT("Significance"));
	Sig->SetAutoActivate(false);
	Sig->OffscreenTimeMax = .2; // manage the ticks
	Sig->IsOffIfOffscreen = false; // avoid deadlocking the significance
	Sig->TestOcclusion = true;
	Sig->IsOffIfOccluded = false;
}

void ATv00::BeginPlay() {
	Super::BeginPlay();

	// we do need create it, or it won't work. BUT NOT ON THE CONSTRUCTOR OR IT WON'T SAVE!
	// need to set the material for the animcrt manually.
	AnimCrt->Mat = Crt->CreateDynamicMaterialInstance(0);
	AnimCrt->Update(0);
	if (LIKELY(IsValid(AnimCrt->Mat)))
		AnimCrt->Mat->SetScalarParameterValue("Opacity", .7);

	Sig->BindAnim(AnimCrt);

	ULSettings* Settings = ULSettings::Instance(this);
	if (LIKELY(Settings))
		Settings->OnFeatUpdateVisual.AddUniqueDynamic(this, &ATv00::FeatUpdG);
}

void ATv00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ULSettings* Settings = ULSettings::Instance(this);
	if (LIKELY(Settings))
		Settings->OnFeatUpdateVisual.RemoveAll(this);

	if (LIKELY(IsValid(Sig))) {
		Sig->UnbindAnim();
		Sig->Deactivate();
	}
	Sig = nullptr;
	if (LIKELY(AnimCrt)) AnimCrt->SetActive(false);
	if (LIKELY(RndCrt)) RndCrt->SetActive(false);

	Super::EndPlay(EndPlayReason);
}

void ATv00::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);

	const bool _IsOpen = !IsEven();
	Noise->Fade(_IsOpen);

	// reuse code
	FeatUpdV(EFeat::V_STROBE, ULSettings::GetFeatS(this, EFeat::V_STROBE));
}

void ATv00::FeatUpdV(const EFeat Feat, const bool bEnabled) {
	// force instant change if no strobe
	if (LIKELY(Feat != EFeat::V_STROBE)) return; // likely cuz there are more

	const bool _IsOpen = !IsEven();
	if (bEnabled) {
		RndCrt->SetActive(_IsOpen);
		AnimCrt->SetActive(_IsOpen);
		// when turning it off, ensure it's off.
		if (!_IsOpen) AnimCrt->Update(0);
		return;
	}

	RndCrt->SetActive(false);
	AnimCrt->SetActive(false);
	AnimCrt->Update(_IsOpen? 1: 0);
}
