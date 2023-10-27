// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
// based on code from Tom Looman https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Public/Components/SSignificanceComponent.h

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SignificanceManager.h" // i wish i could avoid this but the function call seems to need this

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
class JSIG_API UCSignificance: public UActorComponent {
	GENERATED_BODY()

public:
	
	UCSignificance();
	virtual void Activate(bool bReset) override;

	UPROPERTY(BlueprintAssignable)
	FOnSignificanceChanged OnChanged;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Register();
	void Unregister();

	// TODO should have an override of this?
	// maybe a delegate with retval will cover the interface AND the subclassing
	float Calculate(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint);
	void PostUpdate(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final);
};
