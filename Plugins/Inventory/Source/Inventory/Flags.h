// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Flags.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFlagsOnMod, const FName&, Name, float, Diff, float, Total);

DECLARE_LOG_CATEGORY_CLASS(LogFlags, Log, Log);

class UDataTable;

// TODO rename, and also add docs

// World subsystem to deal with Flags ( float variables for stuff like "has unlocked X door, has passed this point, choice taken = 2, etc)
UCLASS(Blueprintable, BlueprintType, Category="Flags")
class INVENTORY_API UFlags : public UWorldSubsystem {
	GENERATED_BODY()

public:

	// regular ones ////////

	UFUNCTION(BlueprintCallable, Category="Flags")
	void Mod(const FName& Name, float Diff);

	UFUNCTION(BlueprintCallable, Category="Flags")
	FORCEINLINE float Get(const FName& Name) const {
		const float* const PreFlag = Flags.Find(Name);
		const float Val = PreFlag ? *PreFlag : 0;
		return Val;
	};
	
	// returns a list of items. Warning/KIKEN/Atchung modifying the item might modify the storage. so be careful.
	UFUNCTION(BlueprintCallable, Category="Flags")
	const TMap<FName, float>& GetAllFlags() const { return Flags; }; // can´t forceinline due to const & tmap

	// overrides the current items
	UFUNCTION(BlueprintCallable, Category="Flags")
	FORCEINLINE void SetAllFlags(const TMap<FName, float>& NewFlags) { Flags = NewFlags;};

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Flags")
	bool Has(const FName& Name) {
		if (Name.IsNone()) return false;
		return Flags.Contains(Name);
	};

	
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
