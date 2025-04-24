// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LLearnMan.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLLearnMan, Log, Log)

ALLearnMan* ALLearnMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return nullptr;

	const ALGGameMode* Mode = Cast<ALGGameMode>(World->GetAuthGameMode());
	if (LIKELY(IsValid(Mode)))
		return Mode->LearnMan;

	return Cast<ALLearnMan>(UGameplayStatics::GetActorOfClass(World, ALLearnMan::StaticClass()));
}
