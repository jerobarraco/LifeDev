#include "CAnimatorFade.h"

#include "Kismet/KismetMaterialLibrary.h"

UCAnimatorFade::UCAnimatorFade():Super() {
	MatFName = "Fade";
	MatFStart = 0;
	MatFEnd = 1;
	Duration = 1.f;
	ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBase(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/VoxelFade_DMI.VoxelFade_DMI"));
	MatBase = CMatBase.Object;
	Curve = nullptr; // remove the interact curve.
}

void UCAnimatorFade::BeginPlay() {
	Super::BeginPlay();
	if (Meshes.Num()<1) return;
	
	if (!IsValid(Curve) && !CodeCurve.IsBound()) {
		UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
		CodeCurve.BindDynamic(Lib, &UCodeCurveLib::UCodeCurveLib::BOutInQuad);
	}

	if (!IsValid(MatBase)) {
		MatBase = Meshes[0]->GetMaterial(0);
	}
	
	Mat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), MatBase);
	for (UStaticMeshComponent* C: Meshes) {
		if (!IsValid(C)) continue;
		C->SetMaterial(0, Mat);		
	}
	// Since they all share the same material instance i don't even need to have my own "update"
}

