// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
// based on code from Tom Looman https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Public/Components/SSignificanceComponent.h

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SignificanceManager.h" // i wish i could avoid this but the function call seems to need this

#include "CSignificance.generated.h"

// TO use. just enable this plugin and add some CSignificance components to your actors
// Set the tick interval if desired on this subsystem.

UENUM(BlueprintType, Blueprintable)
enum class ESignificance : uint8 {
	Hidden = 0, // Special tier while owning Actor is hidden in-game
	Low = 1,
	Med = 2,
	// No stripping/culling
	High = 3,
	MAX = 4 UMETA(Hidden)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSignificanceChanged, ESignificance, Significance);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(float, FCalcSignificance, const FTransform& , Viewpoint);
DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FCalcLocation);

// Manages the significance of this object
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JSig), meta=(BlueprintSpawnableComponent))
class JSIG_API UCSignificance: public UActorComponent {
	GENERATED_BODY()

public:
	UCSignificance();

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	
	// returns the current sig
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ESignificance GetSignificance() { return Significance; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsHiddenInsignificant = true;

	// Max distance per significance. Distances in square. increasing significance is expected to have decreasing distances.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESignificance, float> DistanceSqr;
	
	// Tick intervals per level. Interval <0 will disable ticks.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESignificance, float> TickIntervals = {
		{ESignificance::Hidden, -1},
	};

	// components to manage (ticks)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<UActorComponent*> Comps;

	// triggered when the significance changes
	UPROPERTY(BlueprintAssignable)
	FOnSignificanceChanged OnChanged;
	// Set this with a callback to a custom significance calculation.
	// When this is set, the CalcLocation is ignored.
	// (on bp use the "Set" node) 
	UPROPERTY(BlueprintReadWrite)
	FCalcSignificance CalcSignificance;
	// Set this with a callback to a custom Location calculation.
	// This location is then used for a location/based significance calculation.
	// (on bp use the "Set" node) 
	UPROPERTY(BlueprintReadWrite)
	FCalcLocation CalcLocation;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Register();
	void Unregister();
	float Calculate(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint);
	void PostUpdate(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final);
	float GetDistanceSignificance(float DistSqr);
	void UpdateTicks();

	UPROPERTY(BlueprintReadOnly, Transient)
	ESignificance Significance = ESignificance::High;
};
