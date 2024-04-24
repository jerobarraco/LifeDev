// Copyright Jerónimo Barraco-Mármol

#include "Picture.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

APicture::APicture():Super() {
	UseAnim = false;
	UseRewardDestroy = false;
	StateNum = 1;
	Texts = { FText::FromString(TEXT("A Picture")) } ;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Var/Picture.Picture"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-12.5,-1.25,0));
	Mesh->SetRelativeRotation(FRotator(0,0,90).Quaternion());
	
	Interact->SetRelativeLocation(FVector(12.5,-7.5,1.25));
	Interact->SetBoxExtent(FVector(15,10,2.5));
	SFX->SetRelativeLocation(FVector(12.5,-7.5,1.25));
	// TODO new 
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC.Paper_SC"));
	SFX_Trigger = CSnd.Object;

	Super::SetMobility(EComponentMobility::Static);
}
