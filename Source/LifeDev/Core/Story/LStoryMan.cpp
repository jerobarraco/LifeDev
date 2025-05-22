// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LStoryMan.h"

#include "Kismet/GameplayStatics.h"

#include "Story/StoryUI.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

ALStoryMan* ALStoryMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld(); 
	if (UNLIKELY(!IsValid(World))) return nullptr;
	
	const ALGGameMode* const GM = Cast<ALGGameMode>(World->GetAuthGameMode());
	return LIKELY(GM) ?
		GM->StoryMan.Get() :
		Cast<ALStoryMan>(UGameplayStatics::GetActorOfClass(O, ALStoryMan::StaticClass()));
}

ALStoryMan::ALStoryMan():Super() {
	static ConstructorHelpers::FClassFinder<UStoryUI>
		CUI(TEXT("/Game/LifeDev/Game/Dialogs/UI/W_LStoryUI"));
	UIClass = LIKELY(CUI.Succeeded()) ? CUI.Class.Get() : UStoryUI::StaticClass(); // .Get is required for compilation
	// this contains a save icon animation that will only be seen during the transition between chapters
	// but since i've intentionally only save at that point, it works.
}
