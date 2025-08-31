// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LButton.h"

#include "LifeDev/Core/Sentry.h"

void ULButton::DoClick_Implementation() {
	const USentry* const Sentry = USentry::Instance(this);
	if (LIKELY(Sentry))
		Sentry->AddHint("LButton", {{"Name", GetNameSafe(this)}});

	Super::DoClick_Implementation();
}
