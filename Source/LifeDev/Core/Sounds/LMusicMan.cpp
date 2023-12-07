// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMusicMan.h"

#include "Kismet/GameplayStatics.h"
#include "Story/Step.h"
#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "Sounds/CSounder.h"

ALMusicMan::ALMusicMan():Super() {
	
	Rain = CreateDefaultSubobject<UCSounder>(TEXT("Rain"));
	Rain->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Rain/Rain01_S.Rain01_S"));
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
		CEnv(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Snd/Ambience/Ambience__Residential__Night__Crickets__Night_Bird-AmbiX_.Ambience__Residential__Night__Crickets__Night_Bird-AmbiX_"));
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

void ALMusicMan::SetRainS(UWorld* W, bool Play) {
	ALMusicMan* const R = Cast<ALMusicMan>(UGameplayStatics::GetActorOfClass(W, ALMusicMan::StaticClass()));
	if (!R) return;
	R->SetRain(Play);
}

void ALMusicMan::SetEnvironS(UWorld* W, bool Play) {
	ALMusicMan* const R = Cast<ALMusicMan>(UGameplayStatics::GetActorOfClass(W, ALMusicMan::StaticClass()));
	if (!R) return;
	R->SetEnviron(Play);
}

void ALMusicMan::BeginPlay() {
	Super::BeginPlay();
	if (!Enabled) return;

	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Get(W);
	if (Flashback) {
		Flashback->OnChange.AddUniqueDynamic(this, &ALMusicMan::SetIntensity);
	}

	UStory* const Story = UStory::Get(W);
	if (Story) {
		Story->OnStart.AddUniqueDynamic(this, &ALMusicMan::SetStep);
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

	Super::EndPlay(EndPlayReason);
}

void ALMusicMan::SetStep(AStep* Step) {
	if (!IsValid(Step)) return;
	if (Step->Music.IsNull()) return;
	
	USoundBase* const Sound = Step->Music.LoadSynchronous();
	if (!IsValid(Sound)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not load music asset=%s"), *Step->Music.GetAssetName());
		return;
	}

	PlayMusic(Sound);
}
