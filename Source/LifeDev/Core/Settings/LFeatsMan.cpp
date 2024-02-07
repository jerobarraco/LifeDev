// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatsMan.h"

#include "LSettings.h"
#include "Engine/PostProcessVolume.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

ALFeatsMan::ALFeatsMan() :Super(){}

void ALFeatsMan::BeginPlay() {
	Super::BeginPlay();
	UWorld* const W = GetWorld();
	if (!W) return;

	AGameModeBase* const AGMB = W->GetAuthGameMode();
    GM = Cast<ALGGameMode>(AGMB);
	
	ULSettings* const S = ULSettings::Instance(W);
	if (S) {
		S->OnFeatUpdateVisual.AddUniqueDynamic(this, &ALFeatsMan::FeatVisualUpdate);
		// S->OnFeatUpdate.RemoveAll(this);
		FeatVisualUpdate(EFeat::V_LUMEN, S->GetFeat(EFeat::V_LUMEN));
		FeatVisualUpdate(EFeat::V_BLUR, S->GetFeat(EFeat::V_BLUR));
	}
}

void ALFeatsMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ULSettings* const S = ULSettings::Instance(GetWorld());
	if (S) {
		S->OnFeatUpdateVisual.RemoveAll(this);
		S->OnFeatUpdate.RemoveAll(this);
	}
	GM = nullptr;
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
	}
}
