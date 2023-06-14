// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Dialogs/Dialogs.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

void ALStep::Start_Implementation() {
	Super::Start_Implementation();
	AGameModeBase* const GameModeBase = GetWorld()->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get();
	if (LGGameMode) {
		LGGameMode->SetCharInputEnabled(InputEnabled);
	}

	Dialogs->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
	FDialogSequence Seq; TArray<FDialog> Diags; TArray<FDialogChar> Chars;
	Dialogs->AddSeqId(SeqId, Seq, Diags, Chars);
}

void ALStep::Stop_Implementation() {
	Super::Stop_Implementation();
	Dialogs->OnDone.RemoveAll(this);
}

void ALStep::BeginPlay() {
	Super::BeginPlay();
	Dialogs = GetWorld()->GetSubsystem<UDialogs>();
}

void ALStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Dialogs = nullptr;
}
