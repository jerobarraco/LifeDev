// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Ppl00.h"

#include "CPplSig.h"

APpl00::APpl00() {
	Sig = CreateDefaultSubobject<UCPplSig>(TEXT("Sig"));
	Sig->SetAutoActivate(true);
}
