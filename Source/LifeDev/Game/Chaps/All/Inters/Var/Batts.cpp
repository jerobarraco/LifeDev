// Copyright Jerónimo Barraco-Mármol

#include "Batts.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

ABatts::ABatts():Super() {
	Texts = { FText::FromString(TEXT("Pick up batteries")) } ;
	RewardFlash = .1;
	AnimEnabled = false;
	StateNum = 1;
	RewardItem = "Batts";
	TriggerDlg = "BattsPick";

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Batts00/Batts00.Batts00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0.5,2,0));
	Mesh->SetRelativeRotation(FRotator(0,-90,90));
	
	MeshB = CreateDefaultSubobject<UCQuickMesh>(TEXT("MeshB"));
	MeshB->SetupAttachment(Mesh);
	MeshB->SetStaticMesh(CMesh.Object);
	MeshB->SetRelativeLocation(FVector(-0.116369,0.143369,-2.243960));
	MeshB->SetRelativeRotation(FRotator(-11,0,0));

	Interact->SetRelativeLocation(FVector(2.522632,-1.030120,-0.728246));
	Interact->SetBoxExtent(FVector(0.704907,4.189342,2.505187));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/Grab_C.Grab_C"));
	SFX_Trigger = CSnd.Object;

	/// Anims
	AnimFade->Meshes.AddUnique(MeshB);	
}
