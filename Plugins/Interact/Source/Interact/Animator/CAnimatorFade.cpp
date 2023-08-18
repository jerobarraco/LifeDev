#include "CAnimatorFade.h"

#include "Kismet/KismetMaterialLibrary.h"


// https://forums.unrealengine.com/t/make-the-camera-look-at-a-ball/345097/6?u=nande

UCAnimatorFade::UCAnimatorFade():Super() {
	MatFName = "Fade";
	MatFStart = 0;
	MatFEnd = 1;
	ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBase(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/VoxelFade_DMI.VoxelFade_DMI"));
	MatBase = CMatBase.Object; 
}

void UCAnimatorFade::BeginPlay() {
	Super::BeginPlay();
	if (Meshes.Num()<1) return;

	Mat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), MatBase);
	for (UStaticMeshComponent* C: Meshes) {
		C->SetMaterial(0, Mat);		
	}
}

void UCAnimatorFade::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);
}
