// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LLearnMan.h"

#include "Interact/CInteractor.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLLearnMan, Log, Log)

namespace LifeDev {
	namespace Learn {
		static const FName InterTrigger("Inter.Trigger");
	}
}

ALLearnMan* ALLearnMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return nullptr;

	const ALGGameMode* Mode = Cast<ALGGameMode>(World->GetAuthGameMode());
	if (LIKELY(IsValid(Mode)))
		return Mode->LearnMan;

	return Cast<ALLearnMan>(UGameplayStatics::GetActorOfClass(World, ALLearnMan::StaticClass()));
}

void ALLearnMan::Init_Implementation(UDataTable* Data) {
	Super::Init_Implementation(Data);

	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCInteractor* const Inter = Cast<UCInteractor>(Char->GetComponentByClass(UCInteractor::StaticClass()));
	if (UNLIKELY(!Inter)) return;
	Inter->OnTrigger.AddUniqueDynamic(this, &ALLearnMan::InterTriger);
	Inter->OnHover.AddUniqueDynamic(this, &ALLearnMan::InterHover);
}

void ALLearnMan::DeInitInter() {
	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCInteractor* const Inter = Cast<UCInteractor>(Char->GetComponentByClass(UCInteractor::StaticClass()));
	if (UNLIKELY(!Inter)) return;
	Inter->OnTrigger.RemoveAll(this);
	Inter->OnHover.RemoveAll(this);
}

void ALLearnMan::DeInit_Implementation() {
	DeInitInter();
	Super::DeInit_Implementation();
}

void ALLearnMan::InterTriger(const UCInteract* const Comp) {
	Hide(LifeDev::Learn::InterTrigger);
	DeInitInter();
}

void ALLearnMan::InterHover(const bool bOn, UCInteract* const Comp) {
	if (!bOn & !Comp) return;
	// TODO have a timer so that i have to look at it for a few seconnds
	Show(LifeDev::Learn::InterTrigger);
}

