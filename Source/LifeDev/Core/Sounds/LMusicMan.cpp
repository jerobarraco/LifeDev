// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMusicMan.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "Story/Step.h"
#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "Sounds/CSounder.h"

ALMusicMan::ALMusicMan():Super() {
	Rain = CreateDefaultSubobject<UCSounder>(TEXT("Rain"));
	Rain->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Env/Rain/Rain01_S.Rain01_S"));
	Rain->SetSound(CSnd.Object);
	Rain->SetAutoActivate(false);
	Rain->bAutoManageAttachment = true;
	Rain->TimeFadeIn = 2;
	Rain->TimeFadeOut = 2;
	Rain->TimeStart = 0;
	Rain->TimeEnd = 120;
	
	Environ = CreateDefaultSubobject<UCSounder>(TEXT("Environ"));
	Environ->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CEnv(TEXT("/Game/LifeDev/Game/Env/Ambience/Ambience__Residential__Night__Crickets__Night_Bird-AmbiX_.Ambience__Residential__Night__Crickets__Night_Bird-AmbiX_"));
	Environ->SetSound(CEnv.Object);
	Environ->SetAutoActivate(false);
	Environ->bAutoManageAttachment = true;
	Environ->TimeFadeIn = 2;
	Environ->TimeFadeOut = 2;
	Environ->TimeStart = 0;
	Environ->TimeEnd = 0;
}

void ALMusicMan::SetRain(bool Play) {
	if (!IsValid(Rain)) return;
	Rain->Fade(Play);
}

void ALMusicMan::SetEnviron(bool On) {
	if (!IsValid(Environ)) return;
	Environ->Fade(On);
}

void ALMusicMan::Fade_Implementation(bool In) {
	// don't fade in if the music is not enabled.
	// only needs to be done in the fade call, so that the actual music is set in the player.
	// in case someone activates the music after the chapter has started.
	// allow to fadeout always (specially since the feature flag toggle will call fadeout)
	if (In && ! ULSettings::GetFeatS(GetWorld(), EFeat::S_MUSIC)) return;
	Super::Fade_Implementation(In);
}

void ALMusicMan::SetRainS(UWorld* W, bool Play) {
	// Might be faster easier to get it from the gamemode
	ALGGameMode* GM = Cast<ALGGameMode>(UGameplayStatics::GetGameMode(W));
	if (!GM) return;
	GM->MusicMan->SetRain(Play);
	// ALMusicMan* const R = Cast<ALMusicMan>(UGameplayStatics::GetActorOfClass(W, ALMusicMan::StaticClass()));
}

void ALMusicMan::BeginPlay() {
	Super::BeginPlay();

	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Get(W);
	if (Flashback) {
		Flashback->OnChange.AddUniqueDynamic(this, &ALMusicMan::SetIntensity);
	}

	UStory* const Story = UStory::Get(W);
	if (Story) {
		Story->OnStart.AddUniqueDynamic(this, &ALMusicMan::SetStep);
	}

	ULSettings* S = ULSettings::Get(W);
	if (S) {
		S->OnFeatUpdateSound.AddUniqueDynamic(this, &ALMusicMan::FeatUpdate);
	}
}

void ALMusicMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Get(W);
	if (Flashback) {
		Flashback->OnChange.RemoveAll(this);
	}

	UStory* const Story = UStory::Get(W);
	if (Story) {
		Story->OnStart.RemoveAll(this);
	}

	ULSettings* S = ULSettings::Get(W);
	if (S) {
		S->OnFeatUpdateSound.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}

void ALMusicMan::FeatUpdate(EFeat Feat, bool bEnabled) {
	if (Feat == EFeat::S_MUSIC) {
		const bool IsPlaying = Player->IsPlaying();
		// start/stop only if it was stopped/started
		if (bEnabled){
			if (!IsPlaying) {
				Fade(true);
			}
		} else {
			if (IsPlaying) {
				Fade(false);
			}
		}
	} else if (Feat == EFeat::S_ENV) {
		SetEnviron(bEnabled);
	}
}

void ALMusicMan::SetStep(AStep* Step) {
	if (!IsValid(Step)) return;
	if (Step->Music.IsNull()) return;
	
	USoundBase* const Sound = Step->Music.LoadSynchronous();
	if (!IsValid(Sound)) {
		UE_LOG(LogTemp, Warning, TEXT("MusicMan.SetStep: Could not load music asset=%s"), *Step->Music.GetAssetName());
		return;
	}

	PlayMusic(Sound);
}
