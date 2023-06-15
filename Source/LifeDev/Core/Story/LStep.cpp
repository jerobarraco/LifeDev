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
}

void ALStep::Stop_Implementation() {
	Super::Stop_Implementation();
	Dialogs->OnDone.RemoveAll(this);
}

void ALStep::PostWait_Implementation() {
	Super::PostWait_Implementation();
	// otherwise show dialogs
	StartDialogs();
}

void ALStep::StartDialogs() {
	if (SeqId.IsNone()) return;
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
	FDialogSequence Seq; TArray<FDialog> Diags; TArray<FDialogChar> Chars;
	Dialogs->AddId(SeqId);
}

void ALStep::BeginPlay() {
	Super::BeginPlay();
	Dialogs = GetWorld()->GetSubsystem<UDialogs>();
}

void ALStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Dialogs = nullptr;
}

void ALStep::PostLoad() {
	Super::PostLoad();
	if (UseFadeTime) {
		WaitTime = FadeTime; 
	}
	
	// avoid finishing earlier if we have dialogs
	if (!SeqId.IsNone()) {
		FinishPostWait = false;
	}
}
