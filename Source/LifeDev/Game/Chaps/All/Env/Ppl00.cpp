// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Ppl00.h"

#include "CPplSig.h"

APpl00::APpl00() {
	UseFade = true; // read begin play
	Sig = CreateDefaultSubobject<UCPplSig>(TEXT("Sig"));
	SetActorHiddenInGame(true);
	Sig->CompsHide.AddUnique(Root);
}

void APpl00::BeginPlay() {
	Super::BeginPlay();
	UseFade = false; // disable fade here, so that the significance doesn't use it.
	// but so we can use it later.
}
