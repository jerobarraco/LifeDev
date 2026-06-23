// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "UObject/Object.h"

#include "ItemLogic.generated.h"

// Base class for the logic of each item 
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Inventory))
class INVENTORY_API UItemLogic: public UObject {
	GENERATED_BODY()

public:
	UItemLogic();

	// warning: this function can sometimes be called during editor time.
	// set an appropriate outer when creating this object and use GetWorld() when needed.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void BeginPlay();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Look();
	virtual void Look_Implementation() {
		UE_LOG(LogTemp, Log, TEXT("ItemLogic.Look: %lu %s"), (int64)this, *Name.ToString());
	}

	// this will get triggered only for self-use
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Use();
	virtual void Use_Implementation() {
		UE_LOG(LogTemp, Log, TEXT("ItemLogic.Use: *%lu. '%s'"), (int64)this, *Name.ToString());
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	FName Name = NAME_None;

protected:
	virtual void PostInitProperties() override;
	// I can't save the world here since it will crash the editor on load for some weird reason
};
