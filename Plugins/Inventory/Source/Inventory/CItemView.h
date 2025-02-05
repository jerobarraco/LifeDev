// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CoreMinimal.h"

#include "CItemView.generated.h"

class UItemViewUI;

// Will be interacting with interact objects
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UCItemView: public UActorComponent {
public:
	GENERATED_BODY()

	UCItemView();

	UFUNCTION(BlueprintCallable)
	void Show(const FName& Name = NAME_None) {}; // stub

	UFUNCTION(BlueprintCallable)
	void Hide() {};

	// maybe i can get away without using a new input mode just by allowing the inventory to forward commands (yikes)
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UItemViewUI> UIClass = nullptr;
};
