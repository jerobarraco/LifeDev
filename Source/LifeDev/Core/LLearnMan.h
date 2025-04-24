// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Inventory/Learn/LearnMan.h"

#include "LLearnMan.generated.h"

UCLASS(Blueprintable, BlueprintType)
class INVENTORY_API ALLearnMan: public ALearnMan {
	GENERATED_BODY()
public:
#pragma region base
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALLearnMan* Instance(const UObject* const O);
#pragma endregion

#pragma region delegates
#pragma endregion

protected:
};
