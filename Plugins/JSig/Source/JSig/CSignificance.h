// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
// inspired by code from Tom Looman https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Public/Components/SSignificanceComponent.h

#pragma once
#include "Components/ActorComponent.h"

#include "SignificanceManager.h" // i wish i could avoid this but the function call seems to need this
#include "JSigTypes.h"

#include "CSignificance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSignificanceChanged, const ESigValue, Significance, const ESigValue, SignificanceOld);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(ESigValue, FCalcSignificance, const FTransform& , Viewpoint);
DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FCalcLocation);

// Manages the significance of the owner actor
// To use, just enable the plugin and a CSignificance component to your actors.
// See properties in the SetUp category.
// You can also override the calculation and/or location (CalcSignificance, CalcLocation),
// and/or bind to the delegates (OnChange) to implement your own logic.
// You can also read the current significance at any point.
//
// The possible significance values are:
//		Off: The object "doesn't matter". Usually the object gets disabled or hidden.
//		Low, Med: Intermediate values.
//		High: Usually the object is at its maximum (quality, speed, etc.).
//
// Note: that this is one of the few objects that have AutoActivate by default.
// You can set this component Active to true/false whenever you want.
// When it's disabled, it won't be updated by the significance system, which saves resources.
// But be careful not get the object stuck in a place where it can't enable back.
// That could happen if you change active on the Actor Tick, but the significance component manages Tick enabled for the actor.
// Also, it's recommended to not spam change this component's Active, as unreal tends to not like that.
// It's ok to call Activate or Deactivate multiple times, just don't toggle it too frequently (once or more per frame).
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JSig), meta=(BlueprintSpawnableComponent))
class JSIG_API UCSignificance: public UActorComponent {
	GENERATED_BODY()

public:
	UCSignificance();

	// only used to bind to.
	UFUNCTION(BlueprintCallable, Category="JSig")
	FORCEINLINE void ActivateNow() { Activate(); }

	virtual void Activate(const bool bReset=false) override;
	virtual void Deactivate() override;

	// override the starting significance. Constructor safe.
	UFUNCTION(BlueprintCallable, Category="JSig")
	FORCEINLINE void SetDefaultSignificance(const ESigValue Sig) { Significance = Sig; SignificanceOld = Sig; }
	// manually sets a significance. this triggers all side-effects. unsafe on constructor.
	UFUNCTION(BlueprintCallable, Category="JSig", meta=(UnsafeDuringActorConstruction))
	FORCEINLINE void SetSignificance(const ESigValue NewSig) { UpdateSig(NewSig); }

	// returns the current sig
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="JSig")
	FORCEINLINE ESigValue GetSignificance() const { return Significance; }

#pragma region options
	static inline bool Debug = false;

	// Whether the update of this component is concurrent or sequential.
	// Concurrent is more performant but if you override the CalcLocation or CalcSignificance it has to be thread safe.
	// Also, it means that there could be many CSignificance components updated concurrently (at the same time),
	// so beware of any inter-object communication or race conditions.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsConcurrent = true;

	// if set, then when the actor is hidden, it will become insignificant (Off).
	// See CompsHide and IsOffIfHidden.
	// Priority = 10 (when set, this takes precedence over the rest)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsOffIfHidden = true;

	// When set to true, will test for occlusion *based on the Visibility channel*.
	// Note this is probably not very cheap and has precedence over Offscreen.
	// if IsOffIfOccluded will set to off, otherwise it will be low.
	// Priority = 9
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool TestOcclusion = false;

	// Only used if TestOcclusion is set.
	// If the object is occluded: (does not affect if not occluded).
	// If this is set, the significance will be Off; otherwise it will be Low.
	// Priority = 9
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsOffIfOccluded = false;

	// Works with OffscreenTimeMax.
	// When offscreen: (Does not affect if on-screen).
	// If this is set, the significance will be Off, otherwise it will be Low.
	// See OffscreenTimeMax. This requires a mesh.
	// Priority = 8
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool IsOffIfOffscreen = false;
	
	// >=0 The seconds since last render, after which, it will become insignificant (Off).
	// <0 is disabled
	// this requires the actor to have a mesh. (a light is not a mesh)
	// if IsOffWhenOffscreen is true, the component will be off, otherwise it will be low.
	// Priority = 8
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float OffscreenTimeMax = 0.5f;
	
	// *Max* distance per significance.
	// Distances are squared. Usually distance to the camera. (Unless calculation is overriden).
	// By default, the significance is Off.
	// So even if you don't specify it, after exceeding the biggest distance, it will become off. 
	// Increasing significance is expected to have decreasing distances (in code).
	// i.e. if you don't set it like that, it will behave oddly.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESigValue, float> DistanceSqr = {
		{ESigValue::High, 500000},
		{ESigValue::Med, 1000000},
		{ESigValue::Low, 5000000},
		{ESigValue::Off, 10000000},
	};

	// Tick intervals per significance.
	// 0 means every frame.
	// <0 means almost never (it will be replaced by a very high value for technical reasons).
	// Higher means less frequent (slower) updates (more cpu saving).
	// It will set the tick interval on the owner actor.
	// To manage more components set them in CompsTick.
	// It won't change "TickEnabled" since that creates a lot of issues (It's recommended you don't change it either).
	// Beware that objects with a TickInterval different from 0 will NOT tick during pause, even if the TickWhenPaused flag is set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TMap<ESigValue, float> TickIntervals = {
		{ESigValue::High, 0},
		{ESigValue::Med, .15},
		{ESigValue::Low, .3},
		{ESigValue::Off, -1},
	};
	// The component used to calculate the distance.
	// If not set, it will use the owner's root.
	// CalcLocation takes precedence.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USceneComponent> Origin;
#pragma endregion

#pragma region comps
	// Components to manage ticks.
	// Components set here, will have it's TickInterval managed by the 'TickIntervals' property.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<TObjectPtr<UActorComponent>> CompsTicks;
	// components to manage activate/deactivate.
	// Not safe to use on Niagara (Use CompsHide instead)
	// The components listed will be deactivated when the significance is Off, and reactivated when it's not Off.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<TObjectPtr<UActorComponent>> CompsActivate;
	// components to manage HiddenInGame, ONLY when the significance is Off.
	// When significance is Off it will set all the components to HiddenInGame
	// otherwise it will unset HiddenInGame.
	// This is affected by: DistanceSqr, IsOffIfOffscreen, IsOffIfHidden, and IsOffIfOccluded.
	// Note: if the root component is set in this array, and IsOffIfHidden is set to true.
	// once it becomes off ONCE, it will STAY off. Since it won't come back from hidden.
	// Probably the same will happen with the rest of IsOffIf* flags.
	// That's by design, beware.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<TObjectPtr<USceneComponent>> CompsHide;
#pragma endregion

#pragma region delegates
	// triggered when the significance changes. Guaranteed to trigger in game thread.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnSignificanceChanged OnChanged;

	// You con optionally set a callback to a custom significance calculation.
	// When this is set, the CalcLocation is ignored.
	// Can be called at a bg thread if unreal decides to (possibly affected by IsConcurrent and the subsystem::useBgThread).
	// Could be called concurrently if IsConcurrent is set (beware race conditions on multiple objects).
	// (on bp use the "Set" node)
	// Priority = 7
	UPROPERTY(BlueprintReadWrite, Transient, Category=SetUp)
	FCalcSignificance CalcSignificance;
	// You can optionally set a callback to a custom Location calculation.
	// This location is then used for a location/based significance calculation.
	// Could be called at a background thread if unreal decides it
	//	(possibly affected by IsConcurrent and the subsystem::useBgThread).
	// Could be called concurrently if IsConcurrent is set
	//	(beware race conditions on multiple objects).
	// This takes precedence over the "Origin" variable.
	// If this is set, it will be used instead of origin.
	// (on bp use the "Set" node, not bind) 
	UPROPERTY(BlueprintReadWrite, Transient, Category=SetUp)
	FCalcLocation CalcLocation;
#pragma endregion

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma region interface
	float Calculate(USignificanceManager::FManagedObjectInfo* const ObjectInfo, const FTransform& Viewpoint);
	void Update(USignificanceManager::FManagedObjectInfo* const Info, const float OldSig, const float Sig, const bool Final);
#pragma endregion

#pragma region utils
	void Register();
	// make sure this gets called only by Deactivate, to ensure a proper re-register.
	void Unregister();
	float GetDistanceSignificance(const float DistSqr);
	// returns true when the actor is not visible (occluded or invalid)
	bool IsOccluded(const AActor* const Owner, const FTransform& Viewpoint) const;
#pragma endregion

#pragma region updates
	void UpdateSig(const ESigValue NewSig); // not on main thread
	/// all these will run on game thread
	
	// finish the update. calling all the update functions and triggering the delegate.
	void ApplyUpdate();
	void UpdateTicks();
	void UpdateActivate();
	void UpdateHidden();
#pragma endregion

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, Category="JSig")
	ESigValue Significance = ESigValue::High;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient, Category="JSig")
	ESigValue SignificanceOld = ESigValue::High;
};
