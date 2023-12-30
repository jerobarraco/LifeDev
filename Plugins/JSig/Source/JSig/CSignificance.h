// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
// based on code from Tom Looman https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Public/Components/SSignificanceComponent.h

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SignificanceManager.h" // i wish i could avoid this but the function call seems to need this

#include "CSignificance.generated.h"

// Defines the different levels of significance.
// Off means no significance at all.
// some things should/could/would be disabled here 
UENUM(BlueprintType, Blueprintable)
enum class ESigValue : uint8 {
	// Disable everything here
	Off = 0,
	Low = 1,
	Med = 2,
	// No stripping/culling
	High = 3,
	MAX = 4 UMETA(Hidden)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSignificanceChanged, ESigValue, Significance);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(ESigValue, FCalcSignificance, const FTransform& , Viewpoint);
DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FCalcLocation);

// Manages the significance of the owner actor
// To use. just enable this plugin and add some CSignificance components to your actors
// Set the tick interval if desired on this subsystem.
// Also set the distance squared
// you can also set the Comps if you want to manage their ticks too
// you can also override the calculation, and bind to the significance change to implement your own tweaks.
// Note: that this is one of the few objects that have autoActivate by default.
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
	FORCEINLINE ESigValue GetSignificance() { return Significance; }

	// whether the update of this component is concurrent or sequential.
	// Concurrent is more performant but if you override the CalcLocation or CalcSignificance it has to be thread safe.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsConcurrent = true;

	// if set, then when the actor is hidden, it will become insignificant (Off).
	// See CompsHide and IsOffIfHidden.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsOffIfHidden = true;

	// when set to true will test for occlusion based on the Visibility channel.
	// if IsOffIfOccluded will set to off, otherwise it will be low.
	// Note this is probably not very cheap and has precedence over offscreen.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool TestOcclusion = false;

	// performs a visibility test.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsOffIfOccluded = false;

	// Works with OffscreenTimeMax.
	// When offscreen, if this is set, it will be off, otherwise it will be low.
	// (unless you've overriden the significance calculation)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsOffIfOffscreen = false;
	
	// >=0 The seconds since last render before becoming insignificant.
	// <0 is disabled
	// this requires the actor to have a mesh. (a light is not a mesh)
	// if IsOffWhenOffscreen is true, the component will be off, otherwise it will be low.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float OffscreenTimeMax = 0.5f;
	
	// Max distance per significance. Distances in square. increasing significance is expected to have decreasing distances.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESigValue, float> DistanceSqr = {
		{ESigValue::High, 500000},
		{ESigValue::Med, 1000000},
		{ESigValue::Low, 5000000},
		{ESigValue::Off, 10000000},
	};
	
	// Tick intervals per level. Interval <0 will disable ticks. 0 means every tick.
	// Higher means less frequent (slower) updates (more cpu saving)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESigValue, float> TickIntervals = {
		{ESigValue::High, 0},
		{ESigValue::Med, .15},
		{ESigValue::Low, .3},
		{ESigValue::Off, -1},
	};

	// components to manage (ticks)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<UActorComponent*> CompsTicks;
	// components to manage activate/deactivate. Not safe to use on Niagara. Use CompsHide
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<UActorComponent*> CompsActivate;
	
	// components to manage hidden, ONLY when the significance is Off.
	// When significance is Off it will set all the components to HiddenInGame
	// otherwise it will unset HiddenInGame.
	// This is affected by: DistanceSqr, IsOffIfOffscreen, IsOffIfHidden, and IsOffIfOccluded.
	// note if the root component is set in this array, and IsOffIfHidden is set to true.
	// once it becomes off ONCE, it will STAY off. Since it won't come back from hidden.
	// Probably the same will happen with the rest of IsOffIf* flags.
	// This is by design, beware.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<USceneComponent*> CompsHide;

	// triggered when the significance changes. Will trigger on game thread.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnSignificanceChanged OnChanged;
	
	// Set this with a callback to a custom significance calculation.
	// When this is set, the CalcLocation is ignored.
	// Can be called at a bg thread if the significance subsystem wants to.
	// (on bp use the "Set" node) 
	UPROPERTY(BlueprintReadWrite, Transient, Category=SetUp)
	FCalcSignificance CalcSignificance;
	// Set this with a callback to a custom Location calculation.
	// This location is then used for a location/based significance calculation.
	// Can be called at a bg thread if the significance subsystem wants to.
	// (on bp use the "Set" node) 
	UPROPERTY(BlueprintReadWrite, Transient, Category=SetUp)
	FCalcLocation CalcLocation;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	/// interface
	float Calculate(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint);
	void Update(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final);
	/// ~
	
	/// utils
	void Register();
	void Unregister();
	float GetDistanceSignificance(float DistSqr);
	// returns true when the actor is not visible (occluded or invalid)
	bool IsOccluded(const AActor* Owner, const FTransform& Viewpoint) const;
	/// ~
	
	/// updates
	/// all these will run on game thread
	
	// finish the update. calling all the update functions and triggering the delegate.
	void ApplyUpdate();
	void UpdateTicks();
	void UpdateActivate();
	void UpdateHidden();
	///~

	UPROPERTY(BlueprintReadOnly, Transient)
	ESigValue Significance = ESigValue::High;
};
