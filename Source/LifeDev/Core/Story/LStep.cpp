// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "LifeDev/Game/Sys/LGGameMode.h"

void ALStep::Start_Implementation() {
	Super::Start_Implementation();
	AGameModeBase* const GameModeBase = GetWorld()->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get();
	if (LGGameMode) {
		LGGameMode->SetCharInputEnabled(InputEnabled);
	}
}

void ALStep::Stop_Implementation() {
	Super::Stop_Implementation();
}
