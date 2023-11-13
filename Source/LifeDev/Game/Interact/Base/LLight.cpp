// Copyright Jerónimo Barraco-Mármol

#include "LLight.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Game/Flashback/Flashback.h"

// better to do light00 first then extract this one
ALLight::ALLight():Super() {
	// a bit dangerous to do on here. since it will execute before the constructor of the children
	ALLight::SetMobility(EComponentMobility::Static);
	// by default is just a static light.
	Super::SetEnabled(false);
	AnimEnabled = true;
	StateNum = 2;
	Texts = {FText::FromString(TEXT("Turn on")), FText::FromString(TEXT("Turn off"))};
	
	// would trash the materials.
	UseRewardFade = false; 
	AnimFade->Meshes.Empty();
	
	// Texts = {FText::FromString("PickUp")};
	// static ConstructorHelpers::FObjectFinder<USoundBase>
		// CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/LLights/LLights.LLights"));
	// SFX_Trigger = CSnd.Object;
	// RewardFlash = .1;

	Rnd = CreateDefaultSubobject<UCRandomizer>(TEXT("Rnd"));
	Rnd->SetAutoActivate(false);

	SetState(1); // on
}

void ALLight::SetFBFlicker(float NewFBFlicker) {
	FlickrOnFB = NewFBFlicker;
	if (FlickrOnFB<0) {
		Rnd->Deactivate();
		return;
	}

	AnimEnabled = true;
	UFlashback* const Fb = UFlashback::Get(GetWorld());
	if (!Fb) return;
	
	Fb->OnChange.AddUniqueDynamic(this, &ALLight::SetFB);
	Rnd->OnTriggerVal.AddUniqueDynamic(this, &ALLight::Flicker);
}

void ALLight::BeginPlay() {
	Super::BeginPlay();
	
	SetFBFlicker(FlickrOnFB);
}

void ALLight::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UFlashback* const Fb = UFlashback::Get(GetWorld());
	if (Fb) {
		Fb->OnChange.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ALLight::Flicker(float Val) {
	// magically will make it flicker and get back to where it was.
    Anim->IsBouncing = true;
	// turn on if it was off
	Anim->IsReversed = StateNum>1 && State != 0; 
	Anim->Play();
}

void ALLight::SetFB(float Value) {
	// activate and deactivate only run if needed.
	if (Value > FlickrOnFB){
		Rnd->Activate();
	} else {
		Rnd->Deactivate();
	}
}
