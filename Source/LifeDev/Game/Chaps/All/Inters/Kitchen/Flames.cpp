// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Flames.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "CQuickMesh.h"

#include "Interact/CInteract.h"

AFlames::AFlames() {
	StateNum = 2;
	UseAnim = false;
	UseFade = false;
	IsLocked = true;
	// UseAutoActivate = false; // the default

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CFlame(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Flames/Flame_NS"));

	constexpr FVector Poss[8] = {
	};
	// flames
	for (uint8 i= 0; i<8; ++i) {
		const FString SName = TEXT("Flame_") + FString::FromInt(i);
		UNiagaraComponent* Comp = CreateDefaultSubobject<UNiagaraComponent>(FName(SName));
		if (UNLIKELY(!Comp)) continue;
		Comp->SetupAttachment(Interact);
		// Comp->SetRelativeLocation(Poss[i]);
		Comp->SetAsset(CFlame.Object);
		// Comp->SetUseAutoManageAttachment(true);
		// Comp->SetAutoActivate(false);
		Comp->SetAutoActivate(true); // debug
		Flames.Add(Comp);
	}
}

void AFlames::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	for (const TObjectPtr<UNiagaraComponent>& N: Flames) {
		if (UNLIKELY(!N)) continue;
		N->SetActive(NewState == 1);
	}
}

