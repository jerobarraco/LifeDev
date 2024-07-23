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
#include "LifeDev/Game/Snd/CLSounder.h"
#include "LifeDev/Game/Sys/Consts/ConstFlags.h"

ALLight::ALLight():Super() {
	UseAnim = true;
	FlickrOnFB = .7;
	StateNum = 2;
	Texts = { FText::FromString(TEXT("Turn On")), FText::FromString(TEXT("Turn Off")) };
	
	// these would trash the materials.
	UseRewardDestroy = false;
	AnimFade->Meshes.Empty();

	Trans.Empty(); // force the simpler animation which will reverse the animation.
	Anim->TRoot = nullptr; // by default don't animate meshes
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/JUtils/Curves/NoiseRamp_C.NoiseRamp_C"));
	if (CCurve.Succeeded()) Anim->Curve = CCurve.Object; // by default use noise ranmp

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
	SFX_Trigger = CClick.Object;
	
	/// ~
	// by default is just a static light. disable interaction
	ALLight::SetEnabled(false);
	// a bit dangerous to do on here. since it will execute before the constructor of the children
	ALLight::SetMobility(EComponentMobility::Static);
	Mesh->SetCastAllShadows(true);
}

void ALLight::StopFBFlicker() {
	UFlashback* const Fb = UFlashback::Instance(GetWorld());
	Rnd->Deactivate();
	Fb->OnChange.RemoveDynamic(this, &ALLight::SetFB);
}

void ALLight::SetFBFlicker(float NewFBFlicker) {
	UWorld* const W = GetWorld();
	if (!W) return;
	
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
	if (!Fb) return;
	Fb->OnChange.AddUniqueDynamic(this, &ALLight::SetFB);
	// manually update it in case the flag was toggled or the fb was already high
	SetFB(Fb->GetVal());
}

void ALLight::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!World) return;

	ULSettings* const Settings = ULSettings::Instance(World);
	if (!Settings) return;
	
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
	World->GetTimerManager().SetTimerForNextTick(this, &ALLight::TurnOn);
}

void ALLight::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// deactivate the rnd first as it could trigger the rest
	if (IsValid(Rnd)) Rnd->Deactivate();

	if (IsValid(Sig)) {
		Sig->UnbindAnim();
		Sig->Deactivate();
	}

	if (IsValid(Anim)) {
		Anim->OnUpdate.RemoveAll(this);
		Anim->OnBegin.RemoveAll(this);
		Anim->OnEnd.RemoveAll(this);
		Anim->Deactivate();
	}
	
	UWorld* const W = GetWorld();
	if (!W) return;

	UFlashback* const Fb = UFlashback::Instance(W);
	if (Fb) Fb->OnChange.RemoveAll(this);

	ULSettings* const Settings = ULSettings::Instance(W);
	if (Settings) Settings->OnFeatUpdateVisual.AddUniqueDynamic(this, &ALLight::FeatUpdated);

	Super::EndPlay(EndPlayReason);
}

void ALLight::SetState_Implementation(int32 NewState) {
	Super::SetState_Implementation(NewState);
	const bool bClosed = IsClosed();
	// force light change when strobe is disabled
	if (!UseAnim) {
		const float P = bClosed ? 0 : 1;
		AnimUpdate(P, P);
	}

	// count the times you turn off a light. closed == off
	if (Flags) Flags->Mod(LDConsts::Flags::Stats::LightsOn, bClosed ? -1 : 1);
}

void ALLight::AnimUpdate_Implementation(const float P, const float A) {
	if (!SFX_Flicker) return;

	const float v = 1.0-A;
	SFX_Flicker->SetSafeParamFloat("Volume", v);
	// SFX_Flicker->SetVolumeMultiplier(v); // this one doesn't work well every second trigger
}

void ALLight::TurnOn() {
	SetState(1);
}

void ALLight::SetFB(float Value) {
	const bool ShouldFlicker = Value > FlickrOnFB;
	// activate and deactivate. only run if needed.
	Rnd->SetActive(ShouldFlicker, false);
	// don't deactivate the sig here. since there are lights that are can be toggled
	// and the sig is bound to the anim, hence the anim manages it.
}

void ALLight::FeatUpdated(EFeat Feat, bool bEnabled) {
	if (Feat != EFeat::V_STROBE) return;

	UseAnim = bEnabled; // anim is bound to the strobe flag
	if (bEnabled) SetFBFlicker(FlickrOnFB);
	else StopFBFlicker(); // notice this doesn't reset the FlickrOnFB value.
}

void ALLight::FlickerBegin() {
	if (!IsValid(SFX_Flicker)) return;
	SFX_Flicker->Fade(true);

	// using animOnEnd instead of setting the duration.
	// that seems cleaner to me. and more logical. the light controls the cycle.
	// will also work with re-playing an animation mid-way, or looping.
	// SFX_Flicker->SetSafeParamFloat("Duration", Anim->Duration);
}

void ALLight::FlickerEnd() {
	if (!IsValid(SFX_Flicker)) return;
	SFX_Flicker->Fade(false);
}
