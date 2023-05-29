// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep0000.h"

#include "LifeDev/Game/Sys/LGGameMode.h"

ALStep0000::ALStep0000():Super() {
	Name = FName("C0S0");
	DisablePlayer = true;
}

void ALStep0000::Start_Implementation() {
	Super::Start_Implementation();
	if (DisablePlayer) {
		AGameModeBase* const GameModeBase = GetWorld()->GetAuthGameMode();
		ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
		// ALGGameMode* const LGGameMode = ALGGameMode::Get();
		if (LGGameMode) {
			LGGameMode->SetCharInputEnabled(false);
		}
	}
}

void ALStep0000::Stop_Implementation() {
	Super::Stop_Implementation();

	if (DisablePlayer) {
		AGameModeBase* const GameModeBase = GetWorld()->GetAuthGameMode();
		ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
		// ALGGameMode* const LGGameMode = ALGGameMode::Get();
		if (IsValid(LGGameMode)) {
			LGGameMode->SetCharInputEnabled(true);
		}
	}
}
