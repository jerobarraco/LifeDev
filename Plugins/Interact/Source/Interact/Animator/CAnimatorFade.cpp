// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CAnimatorFade.h"

#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogAnimFade, Log, All);

UCAnimatorFade::UCAnimatorFade():Super() {
	MatFName = "Fade";
	MatFStart = 0;
	MatFEnd = 1;
	Duration = 1.f;
	Curve = nullptr; // remove the interact curve.
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBaseNew(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/VoxelFade_MMI"));
	if (LIKELY(CMatBaseNew.Succeeded()))
		MatBase = CMatBaseNew.Object;
}

void UCAnimatorFade::CreateMaterial() {
	UE_LOG(LogAnimFade, Log, TEXT("%hs"), __func__);
	
	if (UNLIKELY(Meshes.Num()<1)) return;
	
	if (!IsValid(Curve) && !CodeCurve.IsBound()) {
		// UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
		// if (LIKELY(Lib))
			CodeCurve.BindDynamic(UCodeCurveLib::Instance(), &UCodeCurveLib::UCodeCurveLib::BOutInQuad);
		// else
			// UE_LOG(LogAnimFade, Warning, TEXT("%hs Could not create the CodeCurve. Skip."),
				// __func__);
	}

	if (!IsValid(MatBase)) {
		UE_LOG(LogAnimFade, Log, TEXT("%hs using mesh 0's material"), __func__);
		MatBase = Meshes[0]->GetMaterial(0);
	}
	Mat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), MatBase);
	if (UNLIKELY(!IsValid(Mat))) {
		UE_LOG(LogAnimFade, Log, TEXT("%hs dynamic mat is invalid. stop."), __func__);
		return;
	}

	UE_LOG(LogAnimFade, Log, TEXT("%hs o=%s mat=%s "),
		__func__, *GetNameSafe(GetOwner()), *Mat->GetName());

	for (UStaticMeshComponent* const C: Meshes) {
		if (UNLIKELY(!IsValid(C))) continue;
		C->SetMaterial(0, Mat);
	}

	// Since they all share the same material instance, i don't even need to have my own "update"
}

