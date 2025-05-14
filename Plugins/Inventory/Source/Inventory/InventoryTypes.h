// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "Engine/DataTable.h"

#include "InventoryTypes.generated.h"

class UItemLogic;

// The base structure for ITEMS
USTRUCT(Blueprintable, BlueprintType)
struct INVENTORY_API FItem: public FTableRowBase {
	GENERATED_BODY()

public:
	// Each trigger will consume one unit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Consumable = true;
	// For things that trigger by themselves (and/or with other objects (e.g. cards))
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DeprecatedProperty))
	bool SelfUsable = false; // TODO merge self-usable and usable
	// For items that can be used with other objects. Tentative, might get removed. (mementos are false)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Usable = true;
	// max allowed number of items, -1 is unlimited.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxCount = -1;
	// Cooldown in seconds. 0 == no cooldown. used for cards. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CoolDown = 0;
	// title of the item
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Title;

	// the description. You can implement how you want to handle multiple ones.
	// it's multiline. hold Shift+Enter for a new line.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	TArray<FText> Descriptions;

	// image
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> Img = nullptr;

	// sound
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<USoundBase> Snd = nullptr;

	// the class for the item logic manager (can be empty/null)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UItemLogic> LogicType = nullptr;

	// Experimental: condition needed to be used or self-used. or empty. 0 is false >0 is true.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UseCondition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FString Comment;

	// Runtime only (transient). used during runtime to keep track of the count
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient)
	int32 Count = 0;
	// Runtime only (transient). the current cool down being applied. used track how much remaining cool down there is.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient)
	float ActiveCoolDown = 0;
	// Runtime only (transient). whether the item is temporarily blocked (used on runtime)
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient)
	bool Locked = false;
	// Runtime only (transient). the logic for this item. created by the inventory on creation.
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Transient)
	TObjectPtr<UItemLogic> Logic = nullptr;
	// note that this object is not manually destroyed. that's all good. it's GC'd.
	// also someone might keep a ref to it :(. We don't say that to in the comments because that's bad anyway O_o
	// anyway uobjects can't be manually destroyed easily.
};
