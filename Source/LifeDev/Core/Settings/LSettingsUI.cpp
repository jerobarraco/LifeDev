// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"

#include "Kismet/GameplayStatics.h"

#include "JUtils/UI/GroupBox.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"

ULSettingsUI::ULSettingsUI():Super() {
	ShowCursor = true;
}

void ULSettingsUI::Show_Implementation() {
	// SetVisibility(ESlateVisibility::Visible);
	Super::Show_Implementation();

	ALMusicMan* const Man = Cast<ALMusicMan>(
	UGameplayStatics::GetActorOfClass(this, ALMusicMan::StaticClass()));
	if (Man) Man->FadeFX(true);

	Load();
}

void ULSettingsUI::Hide_Implementation() {
	ALMusicMan* const Man = Cast<ALMusicMan>(
	UGameplayStatics::GetActorOfClass(this, ALMusicMan::StaticClass()));
	if (Man) Man->FadeFX(false);
	
	Super::Hide_Implementation();
	
	// SetVisibility(ESlateVisibility::Collapsed);
}

void ULSettingsUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
}

void ULSettingsUI::NativeDestruct() {
	Super::NativeDestruct();
}
