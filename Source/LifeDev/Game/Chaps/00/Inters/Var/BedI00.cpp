// Copyright Jerónimo Barraco-Mármol

#include "BedI00.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ABedI00::ABedI00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Bed00/Bed00.Bed00"));
	Mesh->SetStaticMesh(CMesh.Object);
	
	AnimEnabled = false;
	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
	Locked = true;
	Interact->SetEnabled(true);
	Interact->SetRelativeLocation(FVector(102.5,-55,32.5));
	Interact->SetBoxExtent(FVector(102.5,55,32.5));
	
	LockedDlg = "BED00_L";
	Texts = { FText::FromString(TEXT("I really need to sleep...")) };
	RewardFlash = .1;
	UseRewardFade = false;
}
