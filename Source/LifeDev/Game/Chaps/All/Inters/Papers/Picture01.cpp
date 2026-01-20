// Copyright Jerónimo Barraco-Mármol

#include "Picture01.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

APicture01::APicture01():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	StateNum = 1;
	Texts = { NSLOCTEXT("PictureI01", "State0", "A Picture") };

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Picture/Picture01_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeRotation(FRotator(90,0,90).Quaternion());
	Mesh->SetRelativeLocation(FVector(25,0,0));
	Mesh->SetUseDynShadow(true);
	Mesh->bUseAsOccluder = false; // optim. against wall, can't occlude.

	Interact->SetRelativeLocation(FVector(30.000000,-25.000000,7.5));
	Interact->SetBoxExtent(FVector(35,30,7.5));
	SFX->SetRelativeLocation(FVector::ZeroVector);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Drawers/DrawerOpen_C"));
	// SFXTrigger = CSnd.Object;
	SFXs = { CSnd.Object };

	// force using the material from the mesh. (though we don't use reward destroy)
	AnimFade->MatBase = nullptr;
	AnimFade->Mat = nullptr;
	Super::SetMobility(EComponentMobility::Static);
}
