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
	Rnd->IsLooping = true;
	Rnd->Anim = Anim;
	Rnd->UseRandReverse = false; // don´t want to change the state of the light
	Rnd->ValueMin = .1;
	Rnd->ValueMax = 3;
	Rnd->DelayMin = 3;
	Rnd->DelayMax = 15;
	
	SetState(1); // on
}

void ALLight::SetFBFlicker(float NewFBFlicker) {
	FlickrOnFB = NewFBFlicker;
	if (FlickrOnFB<0) {
		Rnd->Deactivate();
		return;
	}

	UFlashback* const Fb = UFlashback::Get(GetWorld());
	if (!Fb) return;
	
	Fb->OnChange.AddUniqueDynamic(this, &ALLight::SetFB);

	AnimEnabled = true;
	// magically will make it flicker and get back to where it was.
	Anim->IsBouncing = true;
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

void ALLight::SetFB(float Value) {
	// activate and deactivate only run if needed.
	if (Value > FlickrOnFB){
		Rnd->Activate(false);
	} else {
		Rnd->Deactivate();
	}
}
