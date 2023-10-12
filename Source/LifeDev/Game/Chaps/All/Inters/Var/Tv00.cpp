// Copyright Jerónimo Barraco-Mármol

#include "Tv00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "JUtils/Actors/CQuickMesh.h"
#include "Sounds/CSounder.h"

ATv00::ATv00():Super() {
	// can't set stuff to static or the button animation won't work :'(
	// so much optimization lost for a single button animation...
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Btn.Tv00-Btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-32.5,27.5,0));
	Mesh->bUseAttachParentBound = true;

	Interact->SetRelativeLocation(FVector(34.815095,-32.798774,23.512333));
	Interact->SetBoxExtent(FVector(35,32,23.658294));
	Interact->SetEnabled(true);
	Texts = { FText::FromString(TEXT("Turn On")), FText::FromString(TEXT("Turn Off")) } ;

	SFX->SetRelativeLocation(FVector(5.329876,21.458294,20));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/Button_Press-007.Button_Press-007"));
	SFX_Open = SOpen.Object;
	SFX_Close = SOpen.Object; // reusing the same
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpenEnd(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
	SFX_OpenEnd = SOpenEnd.Object;

	/// other meshes
	Frame = CreateDefaultSubobject<UCQuickMesh>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CFrame(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Base.Tv00-Base"));
	Frame->SetStaticMesh(CFrame.Object);
	Frame->SetRelativeLocation(FVector(-32.5,27.5,0));
	Frame->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Frame->SetCollisionProfileName("BlockAllDynamic");
	Frame->SetCastAllShadows(true);
	
	// TODo fix glass occluding the crt
	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Glass"));
	Glass->SetupAttachment(Frame);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CGlass(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Screen.Tv00-Screen"));
	Glass->SetStaticMesh(CGlass.Object);
	Glass->SetCastAllShadows(false); // opt
	Glass->bUseAttachParentBound = true; // opt
	
	Crt = CreateDefaultSubobject<UCQuickMesh>(TEXT("Crt"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCrt(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Crt.Tv00-Crt"));
	Crt->SetupAttachment(Frame);
	Crt->SetStaticMesh(CCrt.Object);
	Crt->SetCastAllShadows(false); // opt
	Crt->bUseAttachParentBound = true; // opt
	
	/// anims
	AnimEnabled = true;
	Anim->Duration = .5;
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector(0.0,-2.5,0));

	AnimCrt = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimCrt"));
	AnimCrt->MatVName = "Emissive";
	AnimCrt->MatVStart = FLinearColor::Black;
	AnimCrt->MatVEnd = FLinearColor(10, 10, 10);
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurveMat(TEXT("/JUtils/Curves/Noise_C.Noise_C"));
	AnimCrt->Curve = CCurveMat.Object;
	AnimCrt->Duration = 2; // initial duration
	
	RndCrt = CreateDefaultSubobject<UCRandomizer>(TEXT("RndCrt"));
	RndCrt->Anim = AnimCrt;
	RndCrt->DelayMin = .5;
	RndCrt->DelayMax = 3;
	RndCrt->ValueMin = .5;
	RndCrt->ValueMax = 3;
	RndCrt->IsLooping = true;
	RndCrt->UseRandReverse = true;

	Noise = CreateDefaultSubobject<UCSounder>(TEXT("Noise"));
	Noise->SetupAttachment(IRoot);
	Noise->bAutoManageAttachment = true;
	Noise->SetAutoActivate(false);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSNoise(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/NOISE_EMF_Radiation__Constant_Hum__LCD_TV_Screen_Off.NOISE_EMF_Radiation__Constant_Hum__LCD_TV_Screen_Off"));
	Noise->SetSound(CSNoise.Object);
}

void ATv00::BeginPlay() {
	Super::BeginPlay();
	
	// we do need create it, or it won't work. BUT NOT ON THE CONSTRUCTOR OR IT WON'T SAVE!
	AnimCrt->Mat = Crt->CreateDynamicMaterialInstance(0);
	AnimCrt->Mat->SetVectorParameterValue(AnimCrt->MatVName, FLinearColor::Black);
	AnimCrt->Mat->SetScalarParameterValue("Opacity", .7);
}

void ATv00::Trigger_Implementation() {
	Super::Trigger_Implementation();
	Noise->Fade(IsOpen);
	RndCrt->SetActive(IsOpen);
	AnimCrt->SetActive(IsOpen);
	
	if (IsOpen) {
		// RndCrt->Start();
		// AnimCrt->Play();
	} else {
		// RndCrt->Stop();
		// AnimCrt->Stop();
		// reset
		AnimCrt->Mat->SetVectorParameterValue(AnimCrt->MatVName, FLinearColor::Black);
	}
}
