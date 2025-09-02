// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMusicMan.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Interact/Animator/CAnimator.h"
#include "Interact/Animator/CAnimatorSFX.h"
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

	AnimMusicFX = CreateDefaultSubobject<UCAnimatorSFX>("AnimMusicFX");
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
	AnimMusicFX->Submix = MusicSubmix = CSmx.Object;
	static ConstructorHelpers::FObjectFinder<USoundSubmix>
		CSmxFX (TEXT("/Game/LifeDev/Core/Audio/Mixes/LDSFX.LDSFX"));
	FXSubmix = CSmxFX.Object;

	static ConstructorHelpers::FObjectFinder<USoundEffectSubmixPreset>
		CSFX(TEXT("/Game/LifeDev/Core/Audio/Effects/HPF_FX"));
	AnimMusicFX->FX = MusicFX = CSFX.Object;
	static ConstructorHelpers::FObjectFinder<USoundEffectSubmixPreset>
		CSFX2(TEXT("/Game/LifeDev/Core/Audio/Effects/StereoDelay_FX"));
	FXFX = CSFX2.Object;
}

ALMusicMan* ALMusicMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	// Might be faster easier to get it from the gamemode
	const ALGGameMode* const GM = Cast<ALGGameMode>(W->GetAuthGameMode());
	if (LIKELY(GM && GM->MusicMan)) return GM->MusicMan;

	// the settings menu on intro level needs this. since the gamemode is not ALGGameMode
	return Cast<ALMusicMan>(UGameplayStatics::GetActorOfClass(W, ALMusicMan::StaticClass()));
}

void ALMusicMan::SetRain(const bool Play) const {
	if (UNLIKELY(!IsValid(Rain))) return;

	Rain->Fade(Play);
}

void ALMusicMan::SetEnviron(const bool On) const {
	if (UNLIKELY(!IsValid(Environ))) return;

	const bool Enabled = ULSettings::GetFeatS(this, EFeat::S_ENV) && EnvironOverride;
	UE_LOG(LogTemp, Log, TEXT("LMusicMan::%hs On=%i Enabled=%i Override=%i"),
		__func__, On, Enabled, EnvironOverride);

	// don't enable if it's disabled. but allow to disable it.
	if (UNLIKELY(On && !Enabled)) return;

	Environ->Fade(On);
}

void ALMusicMan::SetEnvironOverride(const bool On) {
	EnvironOverride = On;
}

void ALMusicMan::SetEnvironFB(const float V) const {
	static const FName NFB("FB");
	// calling setSafeParam is safe since it will check if the Environ itself is playing.
	// that way i don't need to check for the S_ENV flag here either
	Environ->SetSafeParamFloat(NFB, V);
}

void ALMusicMan::FadeMusicFX(const bool On) const {
	if (UNLIKELY(!IsValid(AnimMusicFX))) return;

	UE_LOG(LogTemp, Log, TEXT("%hs On=%i"), __func__, On);
	AnimMusicFX->IsReversed = !On;
	AnimMusicFX->Activate(false);
}

void ALMusicMan::Fade_Implementation(const bool In) {
	// don't fade in if the music is not enabled.
	// only needs to be done in the fade call, so that the actual music is set in the player.
	// in case someone activates the music after the chapter has started.
	// allow to fadeout always (specially since the feature flag toggle will call fadeout)
	if (In && !ULSettings::GetFeatS(this, EFeat::S_MUSIC)) {
		SetEnvironFB(0); // important otherwise the Environ will remain stuck at the previous level
		return;
	}

	Super::Fade_Implementation(In);

	// force fb to 0 on the Environ when there's no music playing
	if (!In) SetEnvironFB(0);
	else {
		// reset the flashback when starting. to make sure it's at the right point.
		// only done when fading in to avoid working extra.
		const UFlashback* const Flashback = UFlashback::Instance(this);
		const float Val = LIKELY(Flashback) ? Flashback->GetVal() : 0;
		SetFB(Val);
	}
}

void ALMusicMan::SetFB(float V) {
	// force fb to 0 if the music is not playing.
	// hence handling feature flags for S_MUSIC without having to poll the ULSettings
	if (UNLIKELY(!Player->IsPlaying())) V = 0;

	static FName NInt = "Intensity";
	Player->SetSafeParamFloat(NInt, V);
	SetEnvironFB(V);
	AnimFXFXUpd(V, V);
}

void ALMusicMan::SetRainS(const UWorld* const W, const bool Play) {
	const ALMusicMan* const MM = Instance(W);
	if (UNLIKELY(!MM)) return;
	
	MM->SetRain(Play);
}

void ALMusicMan::FadeS(const UWorld* const W, const bool In) {
	ALMusicMan* const MM = Instance(W);
	if (UNLIKELY(!MM)) return;

	MM->Fade(In);
}

void ALMusicMan::BeginPlay() {
	Super::BeginPlay();

	if (LIKELY(bool(FXSubmix) & bool(FXFX))) {
		// UAudioMixerBlueprintLibrary::AddSubmixEffect(this, FXSubmix, FXFX);
		// AnimFXFXUpd(0, 0);
	}
}

void ALMusicMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	UFlashback* const Flashback = UFlashback::Instance(W);
	if (LIKELY(Flashback)) Flashback->OnChange.RemoveAll(this);

	UStory* const Story = UStory::Instance(W);
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);

	ULSettings* const S = ULSettings::Instance(W);
	if (LIKELY(S)) S->OnFeatUpdateSound.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void ALMusicMan::Init() {
	const UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Instance(W);
	if (LIKELY(Flashback)) {
		Flashback->OnChange.AddUniqueDynamic(this, &ALMusicMan::SetFB);
		// important to reset the value.
		SetFB(Flashback->GetVal()); // doesn't really work if it's not playing. Super::BeginPlay will try to play the music.
	} else
		SetFB(0);

	UStory* const Story = UStory::Instance(W);
	if (LIKELY(Story)) Story->OnStart.AddUniqueDynamic(this, &ALMusicMan::SetStep);

	ULSettings* const S = ULSettings::Instance(W);
	if (LIKELY(S)) S->OnFeatUpdateSound.AddUniqueDynamic(this, &ALMusicMan::FeatUpdate);
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
}

void ALMusicMan::SetStep(AStep* const Step) {
	if (UNLIKELY(!IsValid(Step))) return;
	if (Step->Music.IsNull()) return;

	USoundBase* const Sound = Step->Music.LoadSynchronous();
	if (UNLIKELY(!IsValid(Sound))) {
		UE_LOG(LogTemp, Warning, TEXT("MusicMan.SetStep: Could not load music asset=%s"),
		 *Step->Music.GetAssetName());
		return;
	}

	Play(Sound);
}

void ALMusicMan::AnimFXFXUpd(const float Progress, const float Alpha) {
	if (UNLIKELY(!IsValid(FXSubmix))) return;
	// UE_LOG(LogTemp, Log, TEXT("%hs a=%.5f"), __func__, Alpha);

	FXSubmix->SetSubmixWetLevel(this, Alpha);
	// FXSubmix->SetSubmixDryLevel(this, 1.0-Alpha);
}
