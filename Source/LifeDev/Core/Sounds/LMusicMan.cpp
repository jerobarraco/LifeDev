// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMusicMan.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Interact/Animator/CAnimator.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundSubmix.h"

#include "Story/Step.h"
#include "Story/Story.h"
#include "Sounds/CSounder.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Sounds/CLSounder.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "LifeDev/Core/Consts/ConstSettings.h"
#include "LifeDev/Game/Env/Ghost/GhostPool.h"

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
	
	// set correct attenuation so it's hearable.
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CEnvAttn(LDConsts::Audio::Attns::Env);
	Environ->AttenuationSettings = Rain->AttenuationSettings = CEnvAttn.Object;
	// environ uses the same class as sfx since they behave the same way,
	// and i've already paid a lot of attention trying to mix them.

	AnimMusicFX = CreateDefaultSubobject<UCAnimator>("AnimMusicFX");
	// important otherwise the pause via the settings menu won't work.
	AnimMusicFX->SetTickableWhenPaused(true);
	AnimMusicFX->SetComponentTickInterval(0); // it HAS to be 0, or it will NOT tick on paused.
	AnimMusicFX->Duration = .5;
	// SetTickableWhenPaused(true); // unneeded

	UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
	// unnoticeable but....
	AnimMusicFX->CodeCurve.BindDynamic(Lib,
		&UCodeCurveLib::UCodeCurveLib::UCodeCurveLib::InOutQuart);
	AnimMusicFX->Curve = nullptr;

	static ConstructorHelpers::FObjectFinder<USoundSubmix>
		CSmx (TEXT("/Game/LifeDev/Core/Audio/Mixes/LDMusic.LDMusic"));
	MusicSubmix = CSmx.Object;

	static ConstructorHelpers::FObjectFinder<USoundEffectSubmixPreset>
		CSFX(TEXT("/Game/LifeDev/Core/Audio/Effects/HPF_FX"));
	MusicFX = CSFX.Object;
}

ALMusicMan* ALMusicMan::Instance(const UObject* const O) {
	if (!O) return nullptr;
	const UWorld* const W = O->GetWorld();

	// Might be faster easier to get it from the gamemode
	const ALGGameMode* const GM = Cast<ALGGameMode>(UGameplayStatics::GetGameMode(W));
	return GM ? GM->MusicMan : nullptr;
}

void ALMusicMan::SetRain(const bool Play) {
	if (!IsValid(Rain)) return;

	Rain->Fade(Play);
}

void ALMusicMan::SetEnviron(const bool On) {
	if (!IsValid(Environ)) return;

	const bool Enabled = ULSettings::GetFeatS(this, EFeat::S_ENV) && EnvironOverride;
	UE_LOG(LogTemp, Log, TEXT("LMusicMan::%hs On=%i Enabled=%i"), __func__, On, Enabled);

	// don't enable if it's disabled. but allow disable
	if (On && !Enabled) return;

	Environ->Fade(On);
}

void ALMusicMan::SetEnvironOverride(const bool On) {
	EnvironOverride = On;
}

void ALMusicMan::SetEnvironFB(const float V) {
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

void ALMusicMan::KillGhosts(const bool All) {
	if (GhostPool) GhostPool->Kill(All);
}

void ALMusicMan::Fade_Implementation(const bool In) {
	// don't fade in if the music is not enabled.
	// only needs to be done in the fade call, so that the actual music is set in the player.
	// in case someone activates the music after the chapter has started.
	// allow to fadeout always (specially since the feature flag toggle will call fadeout)
	if (In && !ULSettings::GetFeatS(this, EFeat::S_MUSIC)) {
		SetEnvironFB(0); // important otherwise the Environ will remain stuck ath the previous level
		return;
	}
	Super::Fade_Implementation(In);

	// force fb to 0 on the Environ when there's no music playing 
	if (!In) SetEnvironFB(0);
	else {
		// reset the flashback when starting. to make sure it's at the right point.
		// only done when fading in to avoid working extra.
		const UFlashback* const Flashback = UFlashback::Instance(this);
		if (Flashback) SetFB(Flashback->GetVal());
	}
}

void ALMusicMan::SetFB_Implementation(float V) {
	Super::SetFB_Implementation(V);

	// force fb to 0 if the music is not playing.
	// hence handling feature flags for S_MUSIC without having to poll the ULSettings
	if (!Player->IsPlaying()) V = 0;

	SetEnvironFB(V);
}

void ALMusicMan::SetRainS(const UWorld* const W, const bool Play) {
	ALMusicMan* const MM = Instance(W);
	if (!MM) return;
	
	MM->SetRain(Play);
}

void ALMusicMan::FadeS(const UWorld* const W, const bool In) {
	ALMusicMan* const MM = Instance(W);
	if (!MM) return;

	MM->Fade(In);
}

void ALMusicMan::BeginPlay() {
	Super::BeginPlay();

	const UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Instance(W);
	if (Flashback)
		Flashback->OnChange.AddUniqueDynamic(this, &ALMusicMan::SetFB);

	UStory* const Story = UStory::Instance(W);
	if (Story)
		Story->OnStart.AddUniqueDynamic(this, &ALMusicMan::SetStep);

	ULSettings* const S = ULSettings::Instance(W);
	if (S) {
		S->OnFeatUpdateSound.AddUniqueDynamic(this, &ALMusicMan::FeatUpdate);
		S->OnFeatUpdateEnviron.AddUniqueDynamic(this, &ALMusicMan::FeatUpdate);
	}

	// important to not clip
	if (MusicSubmix)
		AnimFXUpdate(0, 0); //forces wetmix to 0 resets dry to 1

	if (AnimMusicFX) {
		AnimMusicFX->OnUpdate.AddUniqueDynamic(this, &ALMusicMan::AnimFXUpdate);
		AnimMusicFX->OnEnd.AddUniqueDynamic(this, &ALMusicMan::AnimFXEnd);
	}
	
	SetGhosts(S->GetFeat(EFeat::E_GHOSTPOOL));
}

void ALMusicMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	const UWorld* const W = GetWorld();
	if (!W) return;

	UFlashback* const Flashback = UFlashback::Instance(W);
	if (Flashback)
		Flashback->OnChange.RemoveAll(this);

	UStory* const Story = UStory::Instance(W);
	if (Story)
		Story->OnStart.RemoveAll(this);

	ULSettings* const S = ULSettings::Instance(W);
	if (S) {
		S->OnFeatUpdateSound.RemoveAll(this);
		S->OnFeatUpdateEnviron.RemoveAll(this);
	}

	if (AnimMusicFX) {
		AnimMusicFX->OnUpdate.RemoveAll(this);
		AnimMusicFX->OnEnd.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ALMusicMan::SetGhosts(bool bEnabled) {
	if (bEnabled) {
		if (IsValid(GhostPool)) return;
		UWorld* const W = GetWorld();
		if (!W) return;

		GhostPool = Cast<AGhostPool>(W->SpawnActor(AGhostPool::StaticClass()));
		return;
	}

	if (!GhostPool) return;
	GhostPool->Destroy();
	GhostPool = nullptr;
}

void ALMusicMan::FeatUpdate(const EFeat Feat, const bool bEnabled) {
	if (Feat == EFeat::S_MUSIC) {
		const bool IsPlaying = Player->IsPlaying();
		// start/stop only if it was stopped/started. avoid double fade
		if (bEnabled) {
			if (!IsPlaying) Fade(true);
		} else {
			if (IsPlaying) Fade(false);
		}
	} else if (Feat == EFeat::S_ENV)
		SetEnviron(bEnabled);
	else if (Feat == EFeat::E_GHOSTPOOL)
		SetGhosts(bEnabled);
}

void ALMusicMan::SetStep(AStep* const Step) {
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
	// UE_LOG(LogTemp, Log, TEXT("%hs a=%.5f"), __func__, Alpha);

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
