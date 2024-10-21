// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatsMan.h"

#include "Engine/PostProcessVolume.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#include "LSettings.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

ALFeatsMan::ALFeatsMan() :Super() {
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MPC"));
	MPC = CMPC.Succeeded() ? CMPC.Object : nullptr;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CSpeedMat(TEXT("/Game/LifeDev/Game/Flashback/FlashbackVel_MI"));
	SpeedMat = CSpeedMat.Object;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CFBMat(TEXT("/Game/LifeDev/Game/Flashback/Flashback2_MI"));
	FBMat = CFBMat.Object;
}

void ALFeatsMan::LoadMPC() {
	const UWorld* const W = GetWorld();
	if (!W) return;
	
	if (!IsValid(MPC)) {
		UE_LOG(LogTemp, Warning, TEXT("LFeatsMan::%hs Could not get the MPC. Skip"),
				__func__);
		return;
	}
	
	MPCI = W->GetParameterCollectionInstance(MPC);
	UE_CLOG(!IsValid(MPCI), LogTemp, Warning,
		TEXT("LFeatsMan::%hs Could not get the MPCInst. Stop."), __func__);
}

void ALFeatsMan::BeginPlay() {
	Super::BeginPlay();
	const UWorld* const W = GetWorld();
	if (!W) return;

	AGameModeBase* const AGMB = W->GetAuthGameMode();
	GM = Cast<ALGGameMode>(AGMB);
	
	ULSettings* const S = ULSettings::Instance(W);
	if (S) S->OnFeatUpdateVisual.AddUniqueDynamic(this, &ALFeatsMan::FeatVisualUpdate);
	// S->OnFeatUpdate.RemoveAll(this);

	// force initialize
	LoadMPC();
	
	FeatVisualUpdate(EFeat::V_LUMEN, S && S->GetFeat(EFeat::V_LUMEN));
	FeatVisualUpdate(EFeat::V_BLUR, S && S->GetFeat(EFeat::V_BLUR));
	FeatVisualUpdate(EFeat::V_SPEED, S && S->GetFeat(EFeat::V_SPEED));
	FeatVisualUpdate(EFeat::V_STROBE, S && S->GetFeat(EFeat::V_STROBE));
	FeatVisualUpdate(EFeat::V_FLASHBACK, S && S->GetFeat(EFeat::V_FLASHBACK));
}

void ALFeatsMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ULSettings* const S = ULSettings::Instance(GetWorld());
	if (S) {
		S->OnFeatUpdateVisual.RemoveAll(this);
		S->OnFeatUpdate.RemoveAll(this);
	}
	GM = nullptr;
	MPCI = nullptr;
	MPC = nullptr;
	SpeedMat = nullptr; // free as a bird
	FBMat = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALFeatsMan::FeatVisualUpdate(EFeat Feat, bool bEnabled) {
	if (!IsValid(GM) || !IsValid(GM->PostProcess)) return;
	
	// Important:
	// these properties on the editor have a checkbox next to them.
	// i DO need to check them for the engine to pay attention to them,
	// otherwise the changes here make no difference.
	// https://forums.unrealengine.com/t/how-can-i-control-post-processing-volume-settings-using-c/465187/2?u=nande
	// disable lumen on runtime https://forums.unrealengine.com/t/is-there-a-way-to-add-an-option-to-enable-disable-lumen-for-in-game-settings/613756

	APostProcessVolume* const Post = GM->PostProcess;
	if (Feat == EFeat::V_LUMEN) {
		Post->Settings.DynamicGlobalIlluminationMethod =
			bEnabled ?
			EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::None;
		Post->Settings.ReflectionMethod =
			bEnabled ? EReflectionMethod::Lumen : EReflectionMethod::None;
	} else if (Feat == EFeat::V_BLUR) {
		Post->Settings.MotionBlurAmount = bEnabled ? MotionBlurAmount: 0;
		Post->Settings.MotionBlurMax = bEnabled ? MotionBlurMax: 0;
		Post->Settings.SceneFringeIntensity = bEnabled ? FringeIntensity: 0;
	} else if (Feat == EFeat::V_FLASHBACK) {
		if (!FBMat) [[unlikely]] return;
		if (bEnabled)
			Post->Settings.AddBlendable(FBMat, 1);
		else
			Post->Settings.RemoveBlendable(FBMat);
	} else if (MPCI) {
		const float v = bEnabled ? 1: 0;
		if (Feat == EFeat::V_STROBE) 
			MPCI->SetScalarParameterValue("Strobe", v);
		else if (Feat == EFeat::V_SPEED) {
			MPCI->SetScalarParameterValue("Speed", v);
			if (!SpeedMat) return;
			if (bEnabled)
				Post->Settings.AddBlendable(SpeedMat, 1);
			else
				Post->Settings.RemoveBlendable(SpeedMat);
		}
	}
}
