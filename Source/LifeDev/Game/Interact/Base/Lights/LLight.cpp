// Copyright Jerónimo Barraco-Mármol

#include "LLight.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "Inventory/Flags.h"
#include "JSig/CSignificance.h"
#include "Sounds/CSounder.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/CLSignificance.h"
#include "LifeDev/Core/Sounds/CLSounder.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

ALLight::ALLight():Super() {
	// by default is just a static light. disable interaction. (specially since now i have light switches)
	UseAutoActivate = false;
	UseAnim = true;
	FlickrOnFB = .7;
	StateNum = 2;
	Texts = {
		NSLOCTEXT("LLight", "State0", "Turn On"),
		NSLOCTEXT("LLight", "State0", "Turn Off")};
	
	// these two would trash the materials.
	UseRewardDestroy = false;
	AnimFade->Meshes.Empty();
	
	Anim->TRoot = nullptr; // by default don't animate meshes
	// small opt. cap at 30fps since this animates light (and audio) only. does not need to be 60.
	Anim->SetComponentTickInterval(1/30.0);

	Trans.Empty(); // force the simpler animation which will reverse the animation.
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/JUtils/Curves/NoiseRamp_C.NoiseRamp_C"));
	if (LIKELY(CCurve.Succeeded())) Anim->Curve = CCurve.Object; // by default use noise ranmp

	// this will trigger the flicker animation randomly
	Rnd = CreateDefaultSubobject<UCRandomizer>(TEXT("Rnd"));
	Rnd->SetAutoActivate(false); // important since it's feature flagged.
	Rnd->IsLooping = true;
	Rnd->Anim = Anim;
	Rnd->UseAnimValue = true;

	// IMPORTANT: don't want to change the state of the light. these two must be false.
	Rnd->UseAnimRandReverse = false;
	Rnd->UseAnimMirror = false;
	
	// will not jump around. might make the anim longer.
	// Rnd->UseAnimMirror = true;
	Rnd->ValueMin = .3;
	Rnd->ValueMax = 3;
	Rnd->DelayMin = 3;
	Rnd->DelayMax = 15;

	// this is controlling the anim and binds to it
	Sig = CreateDefaultSubobject<UCLSignificance>(TEXT("Sig"));
	// disabled. since a light-source that is behind me might change the light in front of me.
	// only change due to distance
	Sig->OffscreenTimeMax = -1;
	// don't off when offscreen. the light might still be onscreen
	Sig->IsOffIfOffscreen = false;
	Sig->TestOcclusion = false; // source could be occluded but not the whole light
	Sig->IsOffIfOccluded = false;

	SFX_Flicker = CreateDefaultSubobject<UCLSounder>(TEXT("SFX_Flicker"));
	// attaching to the SFX seems nicer. but the attenuation will break.
	SFX_Flicker->SetupAttachment(IRoot);
	SFX_Flicker->TimeFadeIn = .1;
	SFX_Flicker->TimeFadeOut = .1;
	SFX_Flicker->AttenuationSettings = SFX->AttenuationSettings;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSNDFlicker(TEXT("/Game/LifeDev/Game/Inters/Lights/LightFlicker_MS.LightFlicker_MS"));
	SFX_Flicker->Sound = CSNDFlicker.Object;
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CClick(TEXT("/Game/LifeDev/Game/Inters/Generic/Wall_Light_Double_Switch_Off-004.Wall_Light_Double_Switch_Off-004"));
	SFXs = {CClick.Object, CClick.Object};

	/// ~
	// a bit dangerous to do on here. since it will execute before the constructor of the children
	ALLight::SetMobility(EComponentMobility::Static);
	Mesh->SetCastAllShadows(true);
	SetStateNow(1); // start on
}

void ALLight::StopFBFlicker() {
	Rnd->Deactivate();
	UFlashback* const Fb = UFlashback::Instance(this);
	if (LIKELY(Fb)) Fb->OnChange.RemoveDynamic(this, &ALLight::SetFB);
}

void ALLight::SetFBFlicker(const float NewFBFlicker) {
	const UWorld* const W = GetWorld();
	if (LIKELY(!W)) return;
	
	// first deactivate if needed. ALWAYS deactivate (important since this is accessibility).
	if (NewFBFlicker < 0) {
		FlickrOnFB = -1;
		StopFBFlicker();
		return;
	}

	// important to set the value. Notice it happens before the feature check.
	// in case it is enabled during gameplay later.
	FlickrOnFB = NewFBFlicker;

	// Feature flag. important.
	if (!ULSettings::GetFeatS(W, EFeat::V_STROBE)) {
		UE_LOG(LogTemp, Log,
			TEXT("LLigth: %hs. Attempted to set fb-flicker, but A_STROBE flag is disabled. Cancelled."),
			__func__);
		return;
	}

	UFlashback* const Fb = UFlashback::Instance(W);
	if (LIKELY(!Fb)) return;
	Fb->OnChange.AddUniqueDynamic(this, &ALLight::SetFB);
	// manually update it in case the flag was toggled or the fb was already high
	SetFB(Fb->GetVal());
}

void ALLight::BeginPlay() {
	Super::BeginPlay();

	const UWorld* const World = GetWorld();
	if (LIKELY(!World)) return;

	ULSettings* const Settings = ULSettings::Instance(World);
	if (LIKELY(!Settings)) return;
	
	// disable if the flag is disabled. but keep disabled if it was disabled by the parent.
	UseAnim = UseAnim && Settings->GetFeat(EFeat::V_STROBE);

	// optimize the anim. do here since some lights can be toggled
	Sig->BindAnim(Anim);

	// bind nevertheless since it doesn't depend on the fb but on the strobe.
	// the strobe will be set with the feat flag
	Anim->OnUpdate.AddUniqueDynamic(this, &ALLight::AnimUpdate);
	Anim->OnBegin.AddUniqueDynamic(this, &ALLight::FlickerBegin);
	Anim->OnEnd.AddUniqueDynamic(this, &ALLight::FlickerEnd);
	// allow to change the feature flag during runtime
	Settings->OnFeatUpdateVisual.AddUniqueDynamic(this, &ALLight::FeatUpdated);
	
	// reset the system
	SetFBFlicker(FlickrOnFB);

	// don't set the state here. it will break the child. we should not need it
	// World->GetTimerManager().SetTimerForNextTick(this, &ALLight::TurnOn);
}

void ALLight::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// deactivate the rnd first as it could trigger the rest
	if (LIKELY(IsValid(Rnd))) Rnd->Deactivate();

	if (LIKELY(IsValid(Sig))) {
		Sig->UnbindAnim();
		Sig->Deactivate();
	}

	if (LIKELY(IsValid(Anim))) {
		Anim->OnUpdate.RemoveAll(this);
		Anim->OnBegin.RemoveAll(this);
		Anim->OnEnd.RemoveAll(this);
		Anim->Deactivate();
	}
	
	const UWorld* const W = GetWorld();
	if (LIKELY(!W)) return;

	UFlashback* const Fb = UFlashback::Instance(W);
	if (LIKELY(Fb)) Fb->OnChange.RemoveAll(this);

	ULSettings* const Settings = ULSettings::Instance(W);
	if (LIKELY(Settings)) Settings->OnFeatUpdateVisual.AddUniqueDynamic(this, &ALLight::FeatUpdated);

	Super::EndPlay(EndPlayReason);
}

void ALLight::SetStateNow_Implementation(const int32 NewState, const bool UseSFX, const bool UseParts) {
	Super::SetStateNow_Implementation(NewState, UseSFX, UseParts);
	const bool bClosed = IsEven();
	const float P = bClosed ? 0 : 1;
	AnimUpdate(P, P);
}

void ALLight::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	const bool bClosed = IsEven();
	// force light change when strobe is disabled
	if (!UseAnim) { // otherwise super will trigger the anim
		const float P = bClosed ? 0 : 1;
		AnimUpdate(P, P);
	}

	// count the times you turn off a light. closed == off
	if (LIKELY(Flags))
		Flags->Mod(LDConsts::Flags::Stats::Lights::Off, bClosed ? 1 : -1);
}

void ALLight::AnimUpdate_Implementation(const float P, const float A) {
	if (UNLIKELY(!SFX_Flicker)) return;

	const float v = 1.0-A;
	SFX_Flicker->SetSafeParamFloat("Volume", v);
	// SFX_Flicker->SetVolumeMultiplier(v); // this one doesn't work well every second trigger
}

void ALLight::SetFB(const float Value) {
	const bool ShouldFlicker = Value > FlickrOnFB;
	// activate and deactivate. only run if needed.
	Rnd->SetActive(ShouldFlicker, false);
	// don't deactivate the sig here. since there are lights that can be toggled
	// and the sig is bound to the anim, hence the anim manages it.
}

void ALLight::FeatUpdated(const EFeat Feat, const bool bEnabled) {
	if (Feat != EFeat::V_STROBE) return;

	UseAnim = bEnabled; // anim is bound to the strobe flag
	if (bEnabled) SetFBFlicker(FlickrOnFB);
	else StopFBFlicker(); // notice this doesn't reset the FlickrOnFB value.
}

void ALLight::FlickerBegin() {
	if (UNLIKELY(!IsValid(SFX_Flicker))) return;
	SFX_Flicker->Fade(true);

	// using animOnEnd instead of setting the duration.
	// that seems cleaner to me. and more logical. the light controls the cycle.
	// will also work with re-playing an animation mid-way, or looping.
	// SFX_Flicker->SetSafeParamFloat("Duration", Anim->Duration);
}

void ALLight::FlickerEnd() {
	if (UNLIKELY(!IsValid(SFX_Flicker))) return;
	SFX_Flicker->Fade(false);
}
