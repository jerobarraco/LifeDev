// Copyright (C) 2023 - Jeronimo Barraco-Marmol
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Flags.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFlagsOnMod, const FName&, Name, float, Diff, float, Total);

// World subsystem to deal with Flags ( float variables for stuff like "has unlocked X door, has passed this point, choice taken = 2, etc)
UCLASS(Blueprintable, BlueprintType, Category="Flags")
class INVENTORY_API UFlags : public UWorldSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UFlags* Instance(UObject* O);
	
	// regular ones ////////

	// when a flag is set to 0 it will be removed from memory.
	// this is consistent to how Get works
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Set(const FName& Name, float Val = 1);

	UFUNCTION(BlueprintCallable, Category="Flags")
	void Mod(const FName& Name, const float Diff, const bool Log=true);

	UFUNCTION(BlueprintCallable, Category="Flags")
	void Rem(const FName& Name);

	UFUNCTION(BlueprintCallable, Category="Flags")
	FORCEINLINE float Get(const FName& Name, const float Default=0) const {
		if (Name.IsNone()) return 0.0;

		const float* const PreFlag = Flags.Find(Name);
		const float Val = PreFlag ? *PreFlag : Default;
		return Val;
	};
	
	// Returns whether a flag is set. regardless of the value. it can be 0.
	UFUNCTION(BlueprintCallable, Category="Flags")
	FORCEINLINE bool IsSet(const FName& Name) const {
		if (Name.IsNone()) return false;
		// this works because set doesn't remove on 0
		return Flags.Contains(Name);
	};

	// Returns whether a flag is set to >=1.
	// This might be a bit slower than calling Get, unless you are also checking if it's >=1.
	UFUNCTION(BlueprintCallable, Category="Flags")
	FORCEINLINE bool Has(const FName& Name) const {
		const float V = Get(Name);
		return FMath::IsNearlyEqual(V, 1) || V >= 1.0;
	}

	// returns a list of flags. Warning/KIKEN/Atchung. so be careful.
	// mostly used for load and saving.
	UFUNCTION(BlueprintCallable, Category="Flags")
	const TMap<FName, float>& GetAll() const { return Flags; };
	// can't forceinline due to const & tmap

	// overrides the current items. used for load and saving. will trigger mod on all, beware.
	UFUNCTION(BlueprintCallable, Category="Flags")
	void SetAll(const TMap<FName, float>& NewFlags);
	UFUNCTION(BlueprintCallable, Category="Flags")
	void Clear(int32 Reserve=0);

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
