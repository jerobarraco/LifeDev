// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Flames.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interact/CInteract.h"
#include "JUtils/Misc/JUtilsMisc.h"

AFlames::AFlames() {
	StateNum = 2;
	UseAnim = false;
	UseFade = false;
	IsLocked = true;
	// UseAutoActivate = false; // the default

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CFlame(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Flames/Flame_NS"));
	// todo test the positions
	// todo reset on the outliner
	
	const FVector Poss[] = {
		FVector(7.500000,7.500000,0.000000),
		FVector(7.500000,-7.500000,0.000000),
		FVector(12.500000,00000,0.000000),
		FVector(0,12.500000,0.000000),
		FVector(0,-12.500000,0.000000),
		FVector(-7.5,-7.500000,0.000000),
		FVector(-7.5,7.500000,0.000000),
		FVector(-12.5,00000,0.000000),
	};
	const size_t PosN = UJUtilsMisc::ArraySize(Poss);

	// flames
	for (uint8 i= 0; i<PosN; ++i) {
		const FString SName = TEXT("Flame_") + FString::FromInt(i);
		UNiagaraComponent* Comp = CreateDefaultSubobject<UNiagaraComponent>(FName(SName));
		if (UNLIKELY(!Comp)) continue;
		Comp->SetupAttachment(Interact);
		Comp->SetRelativeLocation(Poss[i]);
		Comp->SetAsset(CFlame.Object);
		// Comp->SetUseAutoManageAttachment(true); // TODO re-enable these two. disable autoactivate
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

