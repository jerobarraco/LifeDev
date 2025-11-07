// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Ppl00.h"

#include "CPplSig.h"

APpl00::APpl00() {
	UseFade = true; // read begin play
	Sig = CreateDefaultSubobject<UCPplSig>(TEXT("Sig"));
	SetActorHiddenInGame(true);
	// Sig->CompsHide.AddUnique(Root);
}

void APpl00::BeginPlay() {
	Super::BeginPlay();
	UseFade = false; // disable fade here, so that the significance doesn't use it.
	Sig->SetSignificance(ESigValue::Off);
	// but so we can use it later.
	Sig->OnChanged.AddUniqueDynamic(this, &APpl00::SigChanged);
	Sig->SetActive(true, true);
}

void APpl00::SigChanged(const ESigValue Significance, const ESigValue SignificanceOld) {
	UE_LOG(LogTemp, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(GetOwner()));
	// once it's shown, deactivate.
	if (Significance == ESigValue::High) {
		Sig->Deactivate();
		UseFade = true;
		SetActorHiddenInGame(false);
	}
}
