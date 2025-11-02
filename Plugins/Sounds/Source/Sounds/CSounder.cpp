// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "CSounder.h"

#include "Components/AudioComponent.h"

UCSounder::UCSounder():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	Super::SetAutoActivate(false);

	bAutoManageAttachment = true;
	AutoAttachLocationRule = EAttachmentRule::KeepRelative;

	// static ConstructorHelpers::FObjectFinder<USoundBase>
	// 	CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Rain/Rain01.Rain01"));
	// SFX->SetSound(CSnd.Object);
}

void UCSounder::Fade(const bool In) {
	// calling stopdelayed will actually bring problems when switching musics on the musicman.
	// (presumably stopping the new one). Fadeout is good enough and seems to stop the audio,
	// (i.e. trigger onAudioFinished at the end)
	// Don't use Play(); StopDelayed(TimeFadeOut);
	if (In) {
		const float Time = TimeStartMax <0 ? TimeStartMin :
			FMath::RandRange(TimeStartMin, TimeStartMax);
		FadeIn(TimeFadeIn, 1, Time);
	} else
		FadeOut(TimeFadeOut, 0);
}

void UCSounder::SetSafeParamFloat(const FName& Name, const float V) {
	const bool Invalid = !IsValid(this) || !IsPlaying(); // avoid crash
	
	if (UNLIKELY(Invalid)) return; // avoid crashing
	SetFloatParameter(Name, V);
}
