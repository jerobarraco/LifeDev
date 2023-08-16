// Copyright Jerónimo Barraco-Mármol

#include "Tv00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "JUtils/CQuickMesh.h"

ATv00::ATv00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Btn.Tv00-Btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-32.5,27.5,0));

	// todo
	Interact->SetRelativeLocation(FVector(7.5,-10.5,0.25));
	// todo
	Interact->SetBoxExtent(FVector(7.5,10.5,.250000));
	Interact->SetEnabled(true);
	Texts = { FText::FromString(TEXT("Turn On")), FText::FromString(TEXT("Turn Off")) } ;

	SFX->SetRelativeLocation(FVector::Zero());
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/Button_Press-007.Button_Press-007"));
	SFX_Open = SOpen.Object;
	SFX_Close = SOpen.Object; // reusing the same
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpenEnd(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
	SFX_OpenEnd = SOpenEnd.Object;
	
	/// other meshes
	Frame = CreateDefaultSubobject<UCQuickMesh>(TEXT("Frame"));
	Frame->SetRelativeLocation(FVector(-32.5,27.5,0));
	Frame->SetupAttachment(RootComponent);
	Frame->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Frame->SetCollisionProfileName("BlockAllDynamic");

	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Glass"));
	Glass->SetupAttachment(Frame);
	Glass->SetCastShadow(false); // opt
	Glass->bUseAttachParentBound = true; // opt

	Crt = CreateDefaultSubobject<UCQuickMesh>(TEXT("Crt"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCrt(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Tv00/Tv00-Crt.Tv00-Crt"));
	Crt->SetupAttachment(Frame);
	Crt->SetStaticMesh(CCrt.Object);
	Crt->SetCastShadow(false); // opt
	Crt->bUseAttachParentBound = true; // opt
	// TODO test what would happen if by default the mesh has a material instance and i many instance of this object
	// will all the material instances change?
	// Crt->CreateDynamicMaterialInstance(0); // ensure we have a dynamic material set
	
	/// anims
	AnimEnabled = true;
	Anim->Duration = .6;
	
	AnimCrt = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimCrt"));
	AnimCrt->MatVName = "Emissive";
	AnimCrt->MatVEnd = FLinearColor(10, 10, 10);
	AnimCrt->Mat = Cast<UMaterialInstanceDynamic>(Crt->GetMaterial(0));
	// TODO AnimCrt->Curve = 
	
	RndCrt = CreateDefaultSubobject<UCRandomizer>(TEXT("RndCrt"));
	RndCrt->Anim = AnimCrt;
	RndCrt->DelayMin = .5;
	RndCrt->DelayMax = 3;
	RndCrt->ValueMin = .5;
	RndCrt->ValueMax = 3;
	// TODO start rndcrt on trigger stop on close
// TODO logic for animations
// TODO instance with dialogs
}
