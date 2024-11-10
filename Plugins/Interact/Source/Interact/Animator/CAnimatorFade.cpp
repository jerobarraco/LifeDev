// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorFade.h"

#include "Kismet/KismetMaterialLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogAnimFade, Log, Log);

UCAnimatorFade::UCAnimatorFade():Super() {
	MatFName = "Fade";
	MatFStart = 0;
	MatFEnd = 1;
	Duration = 1.f;
	// TODO no-one seems to be using fade_DMI (old) attempt to remove
	//  * actually the objects that are old, will use it implicitly if they are using fade
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBase(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/VoxelFade_DMI.VoxelFade_DMI"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBaseNew(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/New/VoxelFade_NDMI.VoxelFade_NDMI"));
	
	MatBase = CMatBase.Object;
	Curve = nullptr; // remove the interact curve.
}

void UCAnimatorFade::SetNewMat() {
	UE_LOG(LogAnimFade, Log, TEXT("%hs"), __func__);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBaseNew(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/New/VoxelFade_NDMI.VoxelFade_NDMI"));
	MatBase = CMatBaseNew.Object;
	
	// alternatively tsoftobjectptr
	// StaticLoadObject()
}

void UCAnimatorFade::CreateMaterial() {
	UE_LOG(LogAnimFade, Log, TEXT("%hs"), __func__);
	
	if (Meshes.Num()<1) return;
	
	if (!IsValid(Curve) && !CodeCurve.IsBound()) {
		UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
		CodeCurve.BindDynamic(Lib, &UCodeCurveLib::UCodeCurveLib::BOutInQuad);
	}

	if (!IsValid(MatBase)) {
		UE_LOG(LogTemp, Log, TEXT("%hs using mesh 0's material"), __func__);
		MatBase = Meshes[0]->GetMaterial(0);
	}

	Mat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), MatBase);
	if (!IsValid(Mat)) {
		UE_LOG(LogTemp, Log, TEXT("%hs dynamic mat is invalid. stop."), __func__);
		return;
	}

	// TODO find all the objects with voxelfade_dmi and reexport to _ndmi
	UE_LOG(LogTemp, Log, TEXT("%hs o=%s mat=%s "),
		__func__, *GetNameSafe(GetOwner()), *Mat->GetName());

	for (UStaticMeshComponent* const C: Meshes) {
		if (!IsValid(C)) continue;
		C->SetMaterial(0, Mat);
	}

	// Since they all share the same material instance, i don't even need to have my own "update"
}

