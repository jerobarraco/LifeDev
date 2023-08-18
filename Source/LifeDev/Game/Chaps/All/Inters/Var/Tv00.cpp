// Copyright Jerónimo Barraco-Mármol

#include "Tv00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "JUtils/CQuickMesh.h"

ATv00::ATv00():Super() {
	// TODO set meshes to static

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Btn.Tv00-Btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-32.5,27.5,0));

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
	Mesh->SetStaticMesh(CFrame.Object);
	Frame->SetRelativeLocation(FVector(-32.5,27.5,0));
	Frame->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Frame->SetCollisionProfileName("BlockAllDynamic");

	// TODo fix glass occluding the crt
	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Glass"));
	Glass->SetupAttachment(Frame);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CGlass(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Screen.Tv00-Screen"));
	Glass->SetStaticMesh(CGlass.Object);
	Glass->SetCastShadow(false); // opt
	Glass->bUseAttachParentBound = true; // opt
	Glass->bCastDynamicShadow = false;

	Crt = CreateDefaultSubobject<UCQuickMesh>(TEXT("Crt"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCrt(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Crt.Tv00-Crt"));
	Crt->SetupAttachment(Frame);
	Crt->SetStaticMesh(CCrt.Object);
	Crt->SetCastShadow(false); // opt
	Crt->bUseAttachParentBound = true; // opt
	
	/// anims
	AnimEnabled = true;
	Anim->Duration = .6;
	Anim->IsAdditive = true;
	// TODO set anim (transform)

	AnimCrt = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimCrt"));
	AnimCrt->MatVName = "Emissive";
	AnimCrt->MatVStart = FLinearColor::Black;
	AnimCrt->MatVEnd = FLinearColor(10, 10, 10);
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurveMat(TEXT("/JUtils/Curves/Noise_C.Noise_C"));
	
	// we do need create it, or it won't work
	AnimCrt->Mat = Crt->CreateDynamicMaterialInstance(0);
	AnimCrt->Mat->SetVectorParameterValue(AnimCrt->MatFName, FLinearColor::Black);
	AnimCrt->Mat->SetScalarParameterValue("Opacity", .7);
	RndCrt = CreateDefaultSubobject<UCRandomizer>(TEXT("RndCrt"));
	RndCrt->Anim = AnimCrt;
	RndCrt->DelayMin = .5;
	RndCrt->DelayMax = 3;
	RndCrt->ValueMin = .5;
	RndCrt->ValueMax = 3;
	RndCrt->IsLooping = true;
	RndCrt->UseRandReverse = true;
	
	// TODO instance with dialogs
}

void ATv00::Trigger_Implementation() {
	Super::Trigger_Implementation();
	if (IsOpen) {
		RndCrt->Start();
		AnimCrt->Play();
	} else {
		RndCrt->Stop();
		AnimCrt->Stop();
		AnimCrt->Mat->SetVectorParameterValue(AnimCrt->MatFName, FLinearColor::Black);
	}
}
