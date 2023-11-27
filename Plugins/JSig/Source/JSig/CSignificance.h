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
	High = 3
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSignificanceChanged, ESignificance, Significance);
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FGetSignificance);
DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FGetLocation);

// Manages the significance of this object
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JSig), meta=(BlueprintSpawnableComponent))
class JSIG_API UCSignificance: public UActorComponent {
	GENERATED_BODY()

public:
	UCSignificance();

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	
	// returns the current sicg
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ESignificance GetSignificance() { return Significance; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsHiddenInsignificant = true;

	// distances should be in increasing order.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESignificance, float> Thresholds;
	
	UPROPERTY(BlueprintAssignable)
	FOnSignificanceChanged OnChanged;
	UPROPERTY(BlueprintReadWrite)
	FGetSignificance CalcSignificance;
	UPROPERTY(BlueprintReadWrite)
	FGetLocation CalcLocation;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Register();
	void Unregister();
	float Calculate(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint);
	void PostUpdate(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final);
	float GetDistanceSignificance(float DistSqr);

	UPROPERTY(BlueprintReadOnly, Transient)
	ESignificance Significance = ESignificance::High;
};
