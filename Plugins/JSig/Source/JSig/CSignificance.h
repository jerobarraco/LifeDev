// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
// based on code from Tom Looman https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Public/Components/SSignificanceComponent.h

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SignificanceManager.h" // i wish i could avoid this but the function call seems to need this

#include "CSignificance.generated.h"


UENUM(BlueprintType, Blueprintable)
enum class ESignificance : uint8 {
	Off = 0,
	Low = 1,
	Med = 2,
	// No stripping/culling
	High = 3,
	MAX = 4 UMETA(Hidden)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSignificanceChanged, ESignificance, Significance);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(float, FCalcSignificance, const FTransform& , Viewpoint);
DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FCalcLocation);

// Manages the significance of the owner actor
// To use. just enable this plugin and add some CSignificance components to your actors
// Set the tick interval if desired on this subsystem.
// Also set the distance squared
// you can also set the Comps if you want to manage their ticks too
// you can also override the calculation, and bind to the significance change to implement your own tweaks.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JSig), meta=(BlueprintSpawnableComponent))
class JSIG_API UCSignificance: public UActorComponent {
	GENERATED_BODY()

public:
	UCSignificance();

	// only used to bind to
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ActivateNow() { Activate(); }
	
	virtual void Activate(bool bReset=false) override;
	virtual void Deactivate() override;
	
	// returns the current sig
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ESignificance GetSignificance() { return Significance; }

	// if set, then when the actor is hidden, it will become insignificant (Off)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsOffWhenHidden = true;

	// >=0 The seconds since last render before becoming insignificant.
	// <0 is disabled
	// this requires the actor to have a mesh.(a light is not a mesh)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float RenderSinceMax = 0.5f;
	
	// Max distance per significance. Distances in square. increasing significance is expected to have decreasing distances.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESignificance, float> DistanceSqr = {
		{ESignificance::High, 500000},
		{ESignificance::Med, 1000000},
		{ESignificance::Low, 5000000},
		{ESignificance::Off, 10000000},
	};
	
	// Tick intervals per level. Interval <0 will disable ticks. 0 means every tick.
	// Higher means less frequent (slower) updates (more cpu saving)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESignificance, float> TickIntervals = {
		{ESignificance::High, 0},
		{ESignificance::Med, .15},
		{ESignificance::Low, .3},
		{ESignificance::Off, -1},
	};

	// components to manage (ticks)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<UActorComponent*> CompsTicks;

	// components to manage activate/deactivate
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<UActorComponent*> CompsActivate;

	// triggered when the significance changes
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnSignificanceChanged OnChanged;
	// Set this with a callback to a custom significance calculation.
	// When this is set, the CalcLocation is ignored.
	// (on bp use the "Set" node) 
	UPROPERTY(BlueprintReadWrite, Transient, Category=SetUp)
	FCalcSignificance CalcSignificance;
	// Set this with a callback to a custom Location calculation.
	// This location is then used for a location/based significance calculation.
	// (on bp use the "Set" node) 
	UPROPERTY(BlueprintReadWrite, Transient, Category=SetUp)
	FCalcLocation CalcLocation;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Register();
	void Unregister();
	float Calculate(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint);
	void UpdateActivate();
	void PostUpdate(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final);
	float GetDistanceSignificance(float DistSqr);
	void UpdateTicks();

	UPROPERTY(BlueprintReadOnly, Transient)
	ESignificance Significance = ESignificance::High;
};
