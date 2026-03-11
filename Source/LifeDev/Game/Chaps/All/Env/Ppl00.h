// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "JSig/JSigTypes.h"

#include "LifeDev/Game/Env/Ppl.h"

#include "Ppl00.generated.h"

class UCapsuleComponent;
class UCPplSig;

// A ppl that appears when you're not looking. and says something when you look at it.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APpl00 : public APpl {
	GENERATED_BODY()

public:
	APpl00();

protected:
	virtual void BeginPlay() override;
	virtual void SetActorHiddenInGame(bool NewHidden) override;
	virtual void Look_Implementation() override;
	
	UFUNCTION()
	void SigChanged(const ESigValue Significance, const ESigValue SignificanceOld);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FString ShowCondition;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCPplSig> Sig = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCapsuleComponent> Collider = nullptr;
};
