// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CSignificance.generated.h"


UENUM(BlueprintType)
enum class ESignificance : uint8
{
	Hidden = 0, // Special tier while owning Actor is hidden in-game
	Low = 1,
	Med = 2,
	// No stripping/culling
	High = 3
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSignificanceChanged, ESignificance, Significance);


// Manages the significance of this object
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class JUTILS_API UCSignificance: public UActorComponent {
	GENERATED_BODY()

public:
	
	UCSignificance();
	virtual void Activate(bool bReset) override;

	UPROPERTY(BlueprintAssignable)
	FOnSignificanceChanged OnChanged;
};
