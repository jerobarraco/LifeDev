// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMusicMan.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Interact/Animator/CAnimator.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"

#include "Story/Step.h"
#include "Story/Story.h"
#include "Sounds/CSounder.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Snd/CLSounder.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "LifeDev/Game/Sys/Consts/ConstSettings.h"
#include "Sound/SoundSubmix.h"

ALMusicMan::ALMusicMan():Super() {
	// set the class to the player
	static ConstructorHelpers::FObjectFinder<USoundClass>
		CSClass(LDConsts::Audio::Classes::Music);
	Player->SoundClassOverride = CSClass.Object;
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CSAttn(LDConsts::Audio::Attns::Music);
	Player->AttenuationSettings = CSAttn.Object;
	
	Rain = CreateDefaultSubobject<UCLSounder>(TEXT("Rain"));
	Rain->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Env/Rain/Rain.Rain"));
	Rain->SetSound(CSnd.Object);
	Rain->SetAutoActivate(false);
	Rain->bAutoManageAttachment = true;
	Rain->TimeFadeIn = 2;
	Rain->TimeFadeOut = 2;
	Rain->TimeStartMin = 0;
	Rain->TimeStartMax = 120;

	Environ = CreateDefaultSubobject<UCLSounder>(TEXT("Environ"));
	Environ->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CEnv(TEXT("/Game/LifeDev/Game/Env/Ambience/Ambience"));
	Environ->SetSound(CEnv.Object);
	Environ->SetAutoActivate(false);
	Environ->bAutoManageAttachment = true;
	Environ->TimeFadeIn = 2;
	Environ->TimeFadeOut = 2;
	Environ->TimeStartMin = 0;
	Environ->TimeStartMax = 0;
	// clear the attenuation from the clsounder which would make them not audible.
	
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CEnvAttn(LDConsts::Audio::Attns::Env);
	Environ->AttenuationSettings = Rain->AttenuationSettings = CEnvAttn.Object; 
	// environ uses the same class as sfx since they behave the same way,
	// and i've already paid a lot of attention trying to mix them.

	AnimMusicFX = CreateDefaultSubobject<UCAnimator>("AnimMusicFX");

	static ConstructorHelpers::FObjectFinder<USoundSubmix>
		CSmx (TEXT("/Game/LifeDev/Core/Audio/Mixes/LDMusic.LDMusic"));
	MusicSubmix = CSmx.Object;

	static ConstructorHelpers::FObjectFinder<USoundEffectSubmixPreset>
		CSFX(TEXT("/Game/LifeDev/Core/Audio/Effects/HPF_FX"));
	MusicFX = CSFX.Object;
}

ALMusicMan* ALMusicMan::Instance(UWorld* W) {
	// Might be faster easier to get it from the gamemode
	const ALGGameMode* const GM = Cast<ALGGameMode>(UGameplayStatics::GetGameMode(W));
	return GM ? GM->MusicMan : nullptr;
}

void ALMusicMan::SetRain(bool Play) {
	if (!IsValid(Rain)) return;

	Rain->Fade(Play);
}

void ALMusicMan::SetEnviron(bool On) {
	if (!IsValid(Environ)) return;

	const bool Enabled = ULSettings::GetFeatS(GetWorld(), EFeat::S_ENV) && EnvironOverride;
	// don't enable if it's disabled
	if (On && !Enabled) return;

	Environ->Fade(On);
}

void ALMusicMan::SetEnvironOverride(bool On) {
	EnvironOverride = On;
}

void ALMusicMan::SetEnvironFB(float V) {
	static const FName NFB("FB");
	// calling setSafeParam is safe since it will check if the Environ itself is playing.
	// that way i don't need to check for the S_ENV flag here either
	Environ->SetSafeParamFloat(NFB, V);
}

void ALMusicMan::FadeFX(const bool On) {
	if (!IsValid(AnimMusicFX) || !IsValid(MusicFX) || !IsValid(MusicSubmix)) return;

	UE_LOG(LogTemp, Log, TEXT("%hs On=%i"), __func__, On);
	AnimMusicFX->IsReversed = !On;
	AnimMusicFX->Activate(true);

	if (On)
		UAudioMixerBlueprintLibrary::AddSubmixEffect(
			this, MusicSubmix, MusicFX);
}

void ALMusicMan::Fade_Implementation(bool In) {
	UWorld* const W = GetWorld();
	if (!W) return;

	// don't fade in if the music is not enabled.
	// only needs to be done in the fade call, so that the actual music is set in the player.
	// in case someone activates the music after the chapter has started.
	// allow to fadeout always (specially since the feature flag toggle will call fadeout)
	if (In && ! ULSettings::GetFeatS(W, EFeat::S_MUSIC)) return;
	Super::Fade_Implementation(In);

	// force fb to 0 on the Environ when there's no music playing 
	if (!In) SetEnvironFB(0);
	else {
		// reset the flashback when starting. to make sure it's at the right point.
		// only done when fading in to avoid working extra.
		const UFlashback* const Flashback = UFlashback::Instance(W);
		if (Flashback) SetIntensity(Flashback->GetVal());
	}
}

void ALMusicMan::SetIntensity_Implementation(float V) {
	Super::SetIntensity_Implementation(V);

	// force fb to 0 if the music is not playing.
	// hence handling feature flags for S_MUSIC without having to poll the ULSettings
	if (!Player->IsPlaying()) V = 0;

	SetEnvironFB(V);
}

void ALMusicMan::SetRainS(UWorld* W, bool Play) {
	ALMusicMan* const MM = Instance(W);
	if (!MM) return;
	
	MM->SetRain(Play);
}

void ALMusicMan::FadeS(UWorld* W, bool In) {
	ALMusicMan* const MM = Instance(W);
	if (!MM) return;

	MM->Fade(In);
}

void ALMusicMan::BeginPlay() {
	Super::BeginPlay();

	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Instance(W);
	if (Flashback)
		Flashback->OnChange.AddUniqueDynamic(this, &ALMusicMan::SetIntensity);

	UStory* const Story = UStory::Instance(W);
	if (Story)
		Story->OnStart.AddUniqueDynamic(this, &ALMusicMan::SetStep);

	ULSettings* const S = ULSettings::Instance(W);
	if (S)
		S->OnFeatUpdateSound.AddUniqueDynamic(this, &ALMusicMan::FeatUpdate);

	// important to not clip
	if (MusicSubmix)
		MusicSubmix->SetSubmixWetLevel(this, 0.0);

	if (AnimMusicFX) {
		AnimMusicFX->OnUpdate.AddUniqueDynamic(this, &ALMusicMan::AnimFXUpdate);
		AnimMusicFX->OnEnd.AddUniqueDynamic(this, &ALMusicMan::AnimFXEnd);
	}
}

void ALMusicMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UWorld* const W = GetWorld();
	if (!W) return;

	UFlashback* const Flashback = UFlashback::Instance(W);
	if (Flashback)
		Flashback->OnChange.RemoveAll(this);

	UStory* const Story = UStory::Instance(W);
	if (Story)
		Story->OnStart.RemoveAll(this);

	ULSettings* const S = ULSettings::Instance(W);
	if (S)
		S->OnFeatUpdateSound.RemoveAll(this);

	if (AnimMusicFX) {
		AnimMusicFX->OnUpdate.RemoveAll(this);
		AnimMusicFX->OnEnd.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ALMusicMan::FeatUpdate(EFeat Feat, bool bEnabled) {
	if (Feat == EFeat::S_MUSIC) {
		const bool IsPlaying = Player->IsPlaying();
		// start/stop only if it was stopped/started. avoid double fade
		if (bEnabled){
			if (!IsPlaying) Fade(true);
		} else {
			if (IsPlaying) Fade(false);
		}
	} else if (Feat == EFeat::S_ENV)
		SetEnviron(bEnabled);
}

void ALMusicMan::SetStep(AStep* Step) {
	if (!IsValid(Step)) return;
	if (Step->Music.IsNull()) return;
	
	USoundBase* const Sound = Step->Music.LoadSynchronous();
	if (!IsValid(Sound)) {
		UE_LOG(LogTemp, Warning, TEXT("MusicMan.SetStep: Could not load music asset=%s"),
		 *Step->Music.GetAssetName());
		return;
	}

	PlayMusic(Sound);
}

void ALMusicMan::AnimFXUpdate(const float Progress, const float Alpha) {
	if (!IsValid(MusicSubmix)) return;

	MusicSubmix->SetSubmixWetLevel(this, Alpha);
	MusicSubmix->SetSubmixDryLevel(this, 1.0-Alpha);
}

void ALMusicMan::AnimFXEnd() {
	if (!MusicFX || !MusicSubmix) return;
	// done this way, because i want it to remove it if there's no animmusic.
	const bool Remove = !AnimMusicFX || AnimMusicFX->IsReversed;
	if (Remove)
		UAudioMixerBlueprintLibrary::RemoveSubmixEffect(
			this, MusicSubmix, MusicFX);
}
