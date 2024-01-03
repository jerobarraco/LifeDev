// Copyright Jerónimo Barraco-Mármol

#include "LLight.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "JSig/CSignificance.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/CLSignificance.h"

// better to do light00 first then extract this one
ALLight::ALLight():Super() {
	UseAnim = true;
	FlickrOnFB = .7;
	StateNum = 2;
	Texts = { FText::FromString(TEXT("Turn On")), FText::FromString(TEXT("Turn Off")) };
	Anim->TRoot = nullptr; // by default don't animate meshes
	Trans.Empty(); // force the simpler animation which will reverse the animation.
	
	// these would trash the materials.
	UseRewardFade = false;
	AnimFade->Meshes.Empty();
	
	Rnd = CreateDefaultSubobject<UCRandomizer>(TEXT("Rnd"));
	Rnd->SetAutoActivate(false); // important since it's feature flagged.
	Rnd->IsLooping = true;
	Rnd->Anim = Anim;
	Rnd->UseRandReverse = false; // don´t want to change the state of the light
	Rnd->ValueMin = .1;
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

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CClick(TEXT("/Game/LifeDev/Game/Inters/Generic/Wall_Light_Double_Switch_Off-004.Wall_Light_Double_Switch_Off-004"));
	SFX_Trigger = CClick.Object;
	
	/// ~
	// by default is just a static light.
	SetEnabled(false);
	// a bit dangerous to do on here. since it will execute before the constructor of the children
	ALLight::SetMobility(EComponentMobility::Static);
}

void ALLight::StopFBFlicker() {
	UFlashback* const Fb = UFlashback::Get(GetWorld());
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
	if (!ULSettings::GetFeatS(W, EFeat::A_STROBE)) {
		UE_LOG(LogTemp, Log,
			TEXT("LLigth: %hs. Attempted to set fb-flicker, but A_STROBE flag is disabled. Cancelled."),
			__func__);
		return;
	}

	UFlashback* const Fb = UFlashback::Get(W);
    if (!Fb) return;
	Fb->OnChange.AddUniqueDynamic(this, &ALLight::SetFB);
	// manually update it in case the flag was toggled or the fb was already high
	SetFB(Fb->GetVal());
}

void ALLight::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!World) return;
	ULSettings* const Settings = ULSettings::Get(World);
	if (!Settings) return;

	// optimize the anim. do here since some lights can be toggled
	Sig->BindAnim(Anim);
	Sig->CompsTicks.AddUnique(Anim);
	// bind nevertheless since it doesn't depend on the fb but on the strobe.
	// the strobe will be set with the feat flag
	Anim->OnUpdate.AddUniqueDynamic(this, &ALLight::AnimUpdate);

	// disable if the flag is disabled. but keep disabled if it was disabled by the parent. 
	UseAnim = UseAnim && Settings->GetFeat(EFeat::A_STROBE);
	
	Settings->OnFeatUpdateAccess.AddUniqueDynamic(this, &ALLight::FeatUpdated);
	SetFBFlicker(FlickrOnFB);
	// don't set the state here. it will break the child. we should not need it
    World->GetTimerManager().SetTimerForNextTick(this, &ALLight::TurnOn);
}

void ALLight::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Anim->OnUpdate.RemoveAll(this);

	UWorld* const W = GetWorld();
	if (!W) return;

	UFlashback* const Fb = UFlashback::Get(W);
	if (Fb) {
		Fb->OnChange.RemoveAll(this);
	}

	ULSettings* const Settings = ULSettings::Get(W);
	if (Settings) {
		Settings->OnFeatUpdateAccess.AddUniqueDynamic(this, &ALLight::FeatUpdated);
	}
	Super::EndPlay(EndPlayReason);
}

void ALLight::SetState_Implementation(int32 NewState) {
	Super::SetState_Implementation(NewState);
	// force light change when strobe is disabled
	if (!UseAnim) {
		const float P = IsClosed() ? 0 : 1;
		AnimUpdate(P, P);
	}
}

void ALLight::TurnOn() {
	SetState(1);
}

void ALLight::SetFB(float Value) {
	const bool ShouldFlicker = Value > FlickrOnFB;
	// activate and deactivate only run if needed.
	Rnd->SetActive(ShouldFlicker);
	// don't deactivate the sig here. since there are lights that are can be toggled
	// and the sig is bound to the anim, hence the anim manages it.
}

void ALLight::FeatUpdated(EFeat Feat, bool bEnabled) {
	if (Feat != EFeat::A_STROBE) return;

	UseAnim = bEnabled; // anim is bound to the strobe flag
	if (bEnabled) {
		SetFBFlicker(FlickrOnFB);
	} else {
		StopFBFlicker(); // notice this doesn't reset the FlickrOnFB value.
	}
}
