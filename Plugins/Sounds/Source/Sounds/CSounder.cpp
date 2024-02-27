// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "CSounder.h"

#include "Components/AudioComponent.h"

UCSounder::UCSounder():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	Super::SetAutoActivate(false);
	bAutoManageAttachment = true;
	//
	// static ConstructorHelpers::FObjectFinder<USoundBase>
	// 	CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Rain/Rain01.Rain01"));
	// SFX->SetSound(CSnd.Object);
}

void UCSounder::Fade(bool In) {
	// calling stopdelayed will actually bring problems when switching musics on the musicman.
	// (presumably stopping the new one). Fadeout is good enough and seems to stop the audio,
	// (i.e. trigger onAudioFinished at the end)
	// Don't use Play(); StopDelayed(TimeFadeOut);
	if (In) {
		const float Time = TimeEnd <0 ? TimeStart :
			FMath::RandRange(TimeStart, TimeEnd);
		FadeIn(TimeFadeIn, 1, Time);
	} else {
        FadeOut(TimeFadeOut, 0);
	}
}

void UCSounder::SetSafeParamFloat(const FName& Name, const float V) {
	const bool CanSet = !IsValid(this) || !IsPlaying(); // avoid crash
	
	// avoid crashing
	if (CanSet) return;
	SetFloatParameter(Name, V);
}
