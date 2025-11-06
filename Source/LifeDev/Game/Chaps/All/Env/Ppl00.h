// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "LifeDev/Game/Env/Ppl.h"

#include "Ppl00.generated.h"

class UCPplSig;
// A ppl that appears when you're not looking. and says something when you look at it.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APpl00 : public APpl {
	GENERATED_BODY()

public:
	APpl00();

	
protected:
	void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCPplSig> Sig = nullptr;
};
