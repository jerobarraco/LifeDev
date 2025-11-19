// Copyright Jerónimo Barraco-Mármol

#include "BedI00.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ABedI00::ABedI00():Super() { // TODO base class 
	UseAutoActivate = true;
	UseAnim = false;
	IsLocked = true;
	
	RewardFlash = .1;
	UseRewardDestroy = false;
	
	// LockedDlg = "BED00_L";
	Texts = { NSLOCTEXT("BedI00", "State0", "I really need to sleep...")};
	Interact->SetRelativeLocation(FVector(102.5,-55,32.5));
	Interact->SetBoxExtent(FVector(102.5,55,32.5));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Rooms/Beds/Bed00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetUseDynShadow(true);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSfx(TEXT("/Game/LifeDev/Game/Inters/Rooms/Beds/Creak.Creak"));
	SFXLocked = CSfx.Object;
	SFXs = {CSfx.Object};
	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
}
