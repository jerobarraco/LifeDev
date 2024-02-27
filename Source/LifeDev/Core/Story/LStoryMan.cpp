// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LStoryMan.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

ALStoryMan* ALStoryMan::Instance(UWorld* World) {
	ALGGameMode* const GM = Cast<ALGGameMode>(UGameplayStatics::GetGameMode(World));
	return GM ? GM->StoryMan: nullptr;
}
