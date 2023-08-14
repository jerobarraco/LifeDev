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
	// todo
	Texts = { FText::FromString(TEXT("Turn On")), FText::FromString(TEXT("Turn Off")) } ;

	// todo sfx
	SFX->SetRelativeLocation(FVector::Zero());
	// SFX_Open 
	// SFX_Close
	
	/// other meshes
	// todo meshes for frame, crt, glass
	// ** remember the cast shadows, and static on meshes, and attach parent
	Frame = CreateDefaultSubobject<UCQuickMesh>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);

	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Glass"));
	Glass->SetupAttachment(Frame);
	Glass->SetCastShadow(false); // opt
	Glass->bUseAttachParentBound = true; // opt

	Crt = CreateDefaultSubobject<UCQuickMesh>(TEXT("Crt"));
	Crt->SetupAttachment(Frame);
	Crt->SetCastShadow(false); // opt
	Crt->bUseAttachParentBound = true; // opt
	// TODO test what would happen if by default the mesh has a material instance and i many instance of this object
	// will all the material instances change?
	Crt->CreateDynamicMaterialInstance(0); // ensure we have a dynamic material set
	
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
	
// TODO logic for animations
// TODO instance with dialogs
}
