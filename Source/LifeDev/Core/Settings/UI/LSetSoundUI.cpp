// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetSoundUI.h"

#include "LFeatsGroup.h"

void ULSetSoundUI::Apply_Implementation() {
	Super::Apply_Implementation();
	if (FeatsGroup) FeatsGroup->Apply();
}

void ULSetSoundUI::Load_Implementation() {
	Super::Load_Implementation();
	if (FeatsGroup) FeatsGroup->Load();
}

void ULSetSoundUI::NativeOnInitialized() {
	Super::NativeOnInitialized();

	FeatTexts.Add(EFeat::S_MUSIC, NSLOCTEXT("SetSound", "Music", "Music"));
	FeatTexts.Add(EFeat::S_NOISE, NSLOCTEXT("SetSound", "Noise", "Noise"));
	FeatTexts.Add(EFeat::S_ENV, NSLOCTEXT("SetSound", "Env", "Environment"));
	FeatsGroup->SetUp(FeatTexts); // doing it here to prevent a crashy crash
}
