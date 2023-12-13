// Copyright Jerónimo Barraco-Mármol

#include "LLight.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "JSig/CSignificance.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"

// better to do light00 first then extract this one
ALLight::ALLight():Super() {
	// a bit dangerous to do on here. since it will execute before the constructor of the children
	ALLight::SetMobility(EComponentMobility::Static);

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

	Sig = CreateDefaultSubobject<UCSignificance>(TEXT("Sig"));
	// disabled. since a light-source that is behind me might change the light in front of me.
	Sig->RenderSinceMax = -1;
	// by default is just a static light.
	SetEnabled(false);
}

void ALLight::SetFBFlicker(float NewFBFlicker) {
	UWorld* const W = GetWorld();
	UFlashback* const Fb = UFlashback::Get(W);
    if (!Fb) return;

	// first deactivate if needed. ALWAYS deactivate (important since this is accessibility).
	// notice the this set the flickeronfb value for an actual flicker
	if (NewFBFlicker <= 0) {
		FlickrOnFB = NewFBFlicker;
		Sig->Deactivate();
		Rnd->Deactivate();
		Fb->OnChange.RemoveAll(this);
		return;
	}

	// Feature flag. important.
	if (!ULSettings::GetFeatS(W, EFeat::A_STROBE)) {
		UE_LOG(LogTemp, Log,
			TEXT("LLigth: %hs. Attempted to set fb-flicker, but A_STROBE flag is disabled. Cancelled."),
			__func__);
		return;
	}
	
	Fb->OnChange.AddUniqueDynamic(this, &ALLight::SetFB);
	// TODO rework the fbflicker
}

void ALLight::BeginPlay() {
	Super::BeginPlay();
	if (!ULSettings::GetFeatS(GetWorld(), EFeat::A_STROBE)) {
		UE_LOG(LogTemp, Log,
			TEXT("LLigth: %hs. flag A_STROBE disabled. Disabling the light."),
			__func__);
		SetEnabled(false);
		return;
	}

	// don't even bother with this if not A_STROBE is enabled
	Sig->CompsTicks.AddUnique(Anim);
	SetFBFlicker(FlickrOnFB);
	// don't set the state here. it will break the child. we should not need it
	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &ALLight::TurnOn, 3);
}

void ALLight::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UFlashback* const Fb = UFlashback::Get(GetWorld());
	if (Fb) {
		Fb->OnChange.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ALLight::TurnOn() {
	SetState(1);
}

void ALLight::SetFB(float Value) {
	const bool ShouldFlicker = Value > FlickrOnFB;
	// activate and deactivate only run if needed.
	Rnd->SetActive(ShouldFlicker);
	Sig->SetActive(ShouldFlicker);
}
