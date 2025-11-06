// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "JSig/JSigTypes.h"

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
	virtual void BeginPlay() override;

	UFUNCTION()
	void SigChanged(const ESigValue Significance, const ESigValue SignificanceOld);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCPplSig> Sig = nullptr;
};
