// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatsMan.h"

#include "Engine/PostProcessVolume.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#include "LSettings.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLFeatsMan, Log, Log);

ALFeatsMan::ALFeatsMan() :Super() {
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MPC"));
	MPC = CMPC.Succeeded() ? CMPC.Object : nullptr;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CSpeedMat(TEXT("/Game/LifeDev/Game/Flashback/FlashbackVel_MI"));
	SpeedMat = CSpeedMat.Object;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CFBMat(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MI"));
	FBMat = CFBMat.Object;
}

void ALFeatsMan::LoadMPC() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	
	if (UNLIKELY(!IsValid(MPC))) {
		UE_LOG(LogLFeatsMan, Warning, TEXT("%hs Could not get the MPC. Skip"),
				__func__);
		return;
	}
	
	MPCI = W->GetParameterCollectionInstance(MPC);
	UE_CLOG(!IsValid(MPCI), LogLFeatsMan, Warning,
		TEXT("%hs Could not get the MPCInst. Stop."), __func__);
}

void ALFeatsMan::BeginPlay() {
	Super::BeginPlay();
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	AGameModeBase* const AGMB = W->GetAuthGameMode();
	GM = Cast<ALGGameMode>(AGMB);
	
	ULSettings* const S = ULSettings::Instance(W);
	if (LIKELY(S)) {
		S->OnFeatUpdateVisual.AddUniqueDynamic(this, &ALFeatsMan::FeatUpVisual);
		S->OnFeatUpdateUnreal.AddUniqueDynamic(this, &ALFeatsMan::FeatUpUnreal);
	}

	// force initialize
	LoadMPC();

	FeatUpVisual(EFeat::V_LUMEN, S && S->GetFeat(EFeat::V_LUMEN));
	FeatUpVisual(EFeat::V_BLUR, S && S->GetFeat(EFeat::V_BLUR));
	FeatUpVisual(EFeat::V_SPEED, S && S->GetFeat(EFeat::V_SPEED));
	FeatUpVisual(EFeat::V_STROBE, S && S->GetFeat(EFeat::V_STROBE));
	FeatUpVisual(EFeat::V_FLASHBACK, S && S->GetFeat(EFeat::V_FLASHBACK));
	FeatUpUnreal(EFeat::U_BATCH_TICK, S && S->GetFeat(EFeat::U_BATCH_TICK));
	FeatUpUnreal(EFeat::U_CON_TICK, S && S->GetFeat(EFeat::U_CON_TICK));
}

void ALFeatsMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ULSettings* const S = ULSettings::Instance(GetWorld());
	if (LIKELY(S)) {
		S->OnFeatUpdateVisual.RemoveAll(this);
		S->OnFeatUpdate.RemoveAll(this);
		S->OnFeatUpdateUnreal.RemoveAll(this);
	}

	GM = nullptr;
	MPCI = nullptr;
	MPC = nullptr;
	SpeedMat = nullptr; // free as a bird
	FBMat = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALFeatsMan::FeatUpVisual(const EFeat Feat, const bool bEnabled) {
	if (UNLIKELY(!IsValid(GM) || !IsValid(GM->PostProcess))) return;

	// Important:
	// these properties on the editor have a checkbox next to them.
	// i DO need to check them for the engine to pay attention to them,
	// otherwise the changes here make no difference.
	// https://forums.unrealengine.com/t/how-can-i-control-post-processing-volume-settings-using-c/465187/2?u=nande
	// disable lumen on runtime https://forums.unrealengine.com/t/is-there-a-way-to-add-an-option-to-enable-disable-lumen-for-in-game-settings/613756

	APostProcessVolume* const Post = GM->PostProcess;
	if (Feat == EFeat::V_LUMEN) {
		// needed to allow the flag to override project settings
		Post->Settings.bOverride_DynamicGlobalIlluminationMethod = true;
		Post->Settings.bOverride_ReflectionMethod = true;
		Post->Settings.DynamicGlobalIlluminationMethod =
			bEnabled ?
			EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::None;
		Post->Settings.ReflectionMethod =
			bEnabled ? EReflectionMethod::Lumen : EReflectionMethod::None;
	} else if (Feat == EFeat::V_MLIGHTS) {
		// needed to allow the flag to override project settings
		Post->Settings.bOverride_bMegaLights = true;
		Post->Settings.bMegaLights = bEnabled;
	} else if (Feat == EFeat::V_BLUR) {
		Post->Settings.MotionBlurAmount = bEnabled ? MotionBlurAmount: 0;
		Post->Settings.MotionBlurMax = bEnabled ? MotionBlurMax: 0;
		Post->Settings.SceneFringeIntensity = bEnabled ? FringeIntensity: 0;
	} else if (Feat == EFeat::V_FLASHBACK) {
		if (UNLIKELY(!FBMat)) return;
		if (bEnabled)
			Post->Settings.AddBlendable(FBMat, 1);
		else
			Post->Settings.RemoveBlendable(FBMat);
	} else if (Feat == EFeat::V_NANITE) {
		UE_LOG(LogLFeatsMan, Log, TEXT("%hs Nanite=%i"), __func__, bEnabled);
		IConsoleVariable* const Variable =
			IConsoleManager::Get().FindConsoleVariable(TEXT("r.Nanite"));
		if (UNLIKELY(!Variable)) {
			UE_LOG(LogLFeatsMan, Warning, TEXT("%hs Can't find r.Nanite var. Stop"), __func__);
			return;
		}
		Variable->Set(bEnabled?1:0);
	} else {
		if (UNLIKELY(!MPCI)) return; // on purpose like this, to not make a mistake myself.
		const float v = bEnabled ? 1: 0;
		if (Feat == EFeat::V_STROBE) 
			MPCI->SetScalarParameterValue("Strobe", v);
		else if (Feat == EFeat::V_SPEED) {
			MPCI->SetScalarParameterValue("Speed", v);
			if (UNLIKELY(!SpeedMat)) return;
			if (bEnabled)
				Post->Settings.AddBlendable(SpeedMat, 1);
			else
				Post->Settings.RemoveBlendable(SpeedMat);
		}
	}
	
	// } else if (Feat == EFeat::V_DOF) {
	// // cant do this. it will break the fb post process since it does a blur pass before the fb
	// and can't move the fb to before dof pass without breaking everything.
	// Post->Settings.DepthOfFieldScale = bEnabled ? 1:0;
}

void ALFeatsMan::FeatUpUnreal(const EFeat Feat, const bool bEnabled) {
	if (Feat == EFeat::U_BATCH_TICK) {
		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("tick.AllowBatchedTicks"));
		if (CVar) CVar->Set(bEnabled ? 1 : 0, EConsoleVariableFlags::ECVF_SetByCode);
	} else if (Feat == EFeat::U_CON_TICK) {
		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("tick.AllowConcurrentTickQueue"));
		if (CVar) CVar->Set(bEnabled ? 1 : 0, EConsoleVariableFlags::ECVF_SetByCode);
	}
}
