// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "Flags.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFlagsOnMod, const FName&, Name,
	const float, Diff, const float, Total);

// World subsystem to deal with Flags ( float variables for stuff like "has unlocked X door, has passed this point, choice taken = 2, etc)
UCLASS(Blueprintable, BlueprintType, Category="Flags")
class INVENTORY_API UFlags : public UWorldSubsystem {
	GENERATED_BODY()

public:
	static UFlags* Instance(const UObject* const O);
#pragma region Flag
	// sets the value of a flag
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Set(const FName& Name, const float Val = 1);

	// modifies a flag. optionally logs.
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Mod(const FName& Name, const float Diff, const bool Log=true);

	// removes a flag
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Rem(const FName& Name);

	// returns the current value of a flag, or the default. None always returns 0.
	UFUNCTION(BlueprintCallable, Category="Flags")
	float Get(const FName Name, const float Default=0) const;
	// Returns whether a flag is set. regardless of the value. it can be 0. None always returns false.
	UFUNCTION(BlueprintCallable, Category="Flags")
	bool IsSet(const FName& Name) const;

	// Returns whether a flag is set to >=1.
	// This might be a bit slower than calling Get, unless you are also checking if it's >=1. None always returs false.
	UFUNCTION(BlueprintCallable, Category="Flags")
	bool Has(const FName& Name) const;
#pragma endregion
#pragma region AllFlags
	// returns a list of flags. Warning/KIKEN/Atchung/Peligro. so be careful.
	// mostly used for load and saving.
	UFUNCTION(BlueprintCallable, Category="Flags", meta=(AdvancedDisplay))
	const TMap<FName, float>& GetAll() const { return Flags; }
	// can't forceinline due to const & tmap

	// overrides the current items. used for load and saving. will trigger mod on all, beware.
	UFUNCTION(BlueprintCallable, Category="Flags")
	void SetAll(const TMap<FName, float>& NewFlags);
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Clear(const int32 Reserve=0);
	// prints all flags to log. used for debug. called via console.
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Flags", BlueprintPure=false, meta=(AdvancedDisplay))
	void FlagsDump() const;
#pragma endregion
#pragma region system
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Init() {};
	UFUNCTION(BlueprintCallable, Category="Flags")
	void DeInit() {};
#pragma endregion

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FFlagsOnMod OnMod;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient)
	TMap<FName, float> Flags;
};
