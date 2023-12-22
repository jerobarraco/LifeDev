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

	Sig = CreateDefaultSubobject<UCLSignificance>(TEXT("Sig"));
	// disabled. since a light-source that is behind me might change the light in front of me.
	// only change due to distance
	Sig->RenderSinceMax = -1;
	// don't off when offscreen. the light might still be onscreen 
	Sig->IsOffWhenOffscreen = false;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CClick(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/Wall_Light_Double_Switch_Off-004.Wall_Light_Double_Switch_Off-004"));
	SFX_Trigger = CClick.Object;
	
	/// ~
	// by default is just a static light.
	SetEnabled(false);
	// a bit dangerous to do on here. since it will execute before the constructor of the children
	ALLight::SetMobility(EComponentMobility::Static);
}

void ALLight::SetFBFlicker(float NewFBFlicker) {
	UWorld* const W = GetWorld();
	UFlashback* const Fb = UFlashback::Get(W);
    if (!Fb) return;

	// first deactivate if needed. ALWAYS deactivate (important since this is accessibility).
	// notice the this set the flickeronfb value for an actual flicker
	if (NewFBFlicker <= 0) {
		FlickrOnFB = NewFBFlicker;
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
}

void ALLight::BeginPlay() {
	Super::BeginPlay();
	// optimize the anim
	Sig->BindAnim(Anim);
	Sig->CompsTicks.AddUnique(Anim);
	
	// don't set the state here. it will break the child. we should not need it
    FTimerHandle H;
    GetWorld()->GetTimerManager().SetTimer(H, this, &ALLight::TurnOn, 1);

	const bool CanStrobe = ULSettings::GetFeatS(GetWorld(), EFeat::A_STROBE);
	if (!CanStrobe) {
		UE_LOG(LogTemp, Log,
			TEXT("LLigth: %hs. flag A_STROBE disabled. Disabling the light and UseAnim."),
			__func__);
		UseAnim = false;
		return;
	}

	// don't even bother with these if not A_STROBE is enabled
	Anim->OnUpdate.AddUniqueDynamic(this, &ALLight::AnimUpdate);
	SetFBFlicker(FlickrOnFB);
}

void ALLight::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UFlashback* const Fb = UFlashback::Get(GetWorld());
	if (Fb) {
		Fb->OnChange.RemoveAll(this);
	}

	Anim->OnUpdate.RemoveAll(this);

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
	Sig->SetActive(ShouldFlicker);
}
