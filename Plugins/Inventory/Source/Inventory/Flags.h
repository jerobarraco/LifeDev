// Copyright (C) 2023 - Jeronimo Barraco-Marmol
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Flags.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFlagsOnMod, const FName&, Name, float, Diff, float, Total);
DECLARE_LOG_CATEGORY_CLASS(LogFlags, Log, Log);

// World subsystem to deal with Flags ( float variables for stuff like "has unlocked X door, has passed this point, choice taken = 2, etc)
UCLASS(Blueprintable, BlueprintType, Category="Flags")
class INVENTORY_API UFlags : public UWorldSubsystem {
	GENERATED_BODY()

public:
	static UFlags* Instance(UWorld* W);
	
	// regular ones ////////

	UFUNCTION(BlueprintCallable, Category="Flags")
	void Mod(const FName& Name, float Diff);
	
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Set(const FName& Name, float Val);

	UFUNCTION(BlueprintCallable, Category="Flags")
	FORCEINLINE float Get(const FName& Name) const {
		if (Name.IsNone()) return 0;
		const float* const PreFlag = Flags.Find(Name);
		const float Val = PreFlag ? *PreFlag : 0;
		return Val;
	};
	
	// Returns whether a flag is set to !=0 (includes negatives).  this might be a bit slower than calling Get, unless you are also checking if it's 0
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Flags")
	FORCEINLINE bool IsSet(const FName& Name) const {
		return !FMath::IsNearlyZero(Get(Name));
	};

	// Returns whether a flag is set to >=1. this might be a bit slower than calling Get, unless you are also checking if it's >=1
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Flags")
	FORCEINLINE bool Has(const FName& Name) const {
		return Get(Name) >= 1.0;
	};

	// returns a list of flags. Warning/KIKEN/Atchung. so be careful. mostly used for load and saving.
	UFUNCTION(BlueprintCallable, Category="Flags")
	const TMap<FName, float>& GetAllFlags() const { return Flags; }; // can't forceinline due to const & tmap

	// overrides the current items. used for load and saving.
	UFUNCTION(BlueprintCallable, Category="Flags")
	FORCEINLINE void SetAllFlags(const TMap<FName, float>& NewFlags) { Flags = NewFlags;};

	/// system

	UFUNCTION(BlueprintCallable, Category="Flags")
	void Init() {};
	UFUNCTION(BlueprintCallable, Category="Flags")
	void DeInit() {};
	
	/// ~system

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FFlagsOnMod OnMod;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient)
	TMap<FName, float> Flags;
};
