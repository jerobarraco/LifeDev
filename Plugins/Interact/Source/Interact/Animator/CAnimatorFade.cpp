// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorFade.h"

#include "Kismet/KismetMaterialLibrary.h"

UCAnimatorFade::UCAnimatorFade():Super() {
	MatFName = "Fade";
	MatFStart = 0;
	MatFEnd = 1;
	Duration = 1.f;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBase(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/VoxelFade_DMI.VoxelFade_DMI"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBaseNew(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/New/VoxelFade_NDMI.VoxelFade_NDMI"));
	
	MatBase = CMatBase.Object;
	Curve = nullptr; // remove the interact curve.
}


void UCAnimatorFade::SetNewMat() {
	// TODO test
	// TODO would be good to be able to set this in the constructor.
	static FSoftObjectPath P("/Game/LifeDev/Game/Var/Mats/Voxel/New/VoxelFade_NDMI.VoxelFade_NDMI");
	TSoftObjectPtr<UMaterialInterface> NewMat (P);
	// NewMat.LoadSynchronous()
	MatBase = NewMat.LoadSynchronous();
	// UMaterialInterface* const Mat = FindObjectSafe<UMaterialInterface>(this, 
		// TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/New/VoxelFade_NDMI.VoxelFade_NDMI")
	// );
}

void UCAnimatorFade::SetMaterial() {
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

void UCAnimatorFade::BeginPlay() {
	Super::BeginPlay();
}

