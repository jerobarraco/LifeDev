// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LStoryMan.h"

#include "LifeDev/Game/Sys/LGGameMode.h"

ALStoryMan* ALStoryMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld(); 
	if (UNLIKELY(!IsValid(World))) return nullptr;
	
	ALGGameMode* const GM = Cast<ALGGameMode>(World->GetAuthGameMode());
	return GM ? GM->StoryMan: nullptr;
}
