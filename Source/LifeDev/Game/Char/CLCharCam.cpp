// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "CLCharCam.h"

#include "Inventory/Flags.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

UCLCharCam::UCLCharCam():Super() {
	bUsePawnControlRotation = true; // needed to be able to loop up
}

void UCLCharCam::Init_Implementation() {
	const UFlags* const Flags = UFlags::Instance(this);
	if (!Flags) return;

	const float Foxify =
		-.5 + Flags->Get(LDConsts::Flags::Settings::Global::Foxy); // -.5,.5
	const float FOVMod = FOVFoxy * Foxify;
	FOVMin += FOVMod;
	FOVMax += FOVMod;
	UE_LOG(LogTemp, Log,
		TEXT("%hs FOV foxified. Min=%.4f, Max=%.4f, Mod=%.4f, Foxy=%.4f"),
		__func__, FOVMin, FOVMax, FOVMod, Foxify);
}

void UCLCharCam::BeginPlay() {
	Super::BeginPlay();

	const UWorld* const World = GetWorld();
	if (!World) return;

	ULSettings* const Settings = ULSettings::Instance(this);
	if (Settings) {
		Settings->OnFeatUpdateVisual.AddUniqueDynamic(this, &UCLCharCam::FeatUpdateVisual);
		UseFeatFOV = Settings->GetFeat(EFeat::V_FOV);
	}
	
	SetFB(0); // update fov

	UFlashback* const FB = World->GetSubsystem<UFlashback>();
	if (FB) FB->OnChange.AddUniqueDynamic(this, &UCLCharCam::SetFB);
}

void UCLCharCam::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	const UWorld* const W = GetWorld();
	if (!W) return;

	UFlashback* const FB = W->GetSubsystem<UFlashback>();
	if (FB) FB->OnChange.RemoveAll(this);
	
	ULSettings* const Settings = ULSettings::Instance(this);
	if (Settings) 
		Settings->OnFeatUpdateVisual.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void UCLCharCam::SetFB(const float Value) {
	if (!UseFeatFOV) return;

	SetFieldOfView(FMath::LerpStable(FOVMin, FOVMax, Value));
}

void UCLCharCam::FeatUpdateVisual(const EFeat Feat, const bool bEnabled) {
	UE_LOG(LogTemp, Log, TEXT("%hs, Feat update f=%s on=%i"),
		__func__, *UEnum::GetValueAsString(Feat), bEnabled);

	if (Feat == EFeat::V_FOV) {
		UseFeatFOV = bEnabled;
		if (UseFeatFOV) {
			// force re-set the fb value to set the correct fov
			const UFlashback* const Flashback = UFlashback::Instance(this);
			if (Flashback) SetFB(Flashback->GetVal()); // be aware this also affects the walk speed
		} else {
			SetFieldOfView(FOVMin);
		}
	}
}
