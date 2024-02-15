// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI04.generated.h"

// Chap 04 Puzzle Instance 04 Piano 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI04: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI04();

protected:
	virtual void PostLoad() override;
	// virtual void BeginPlay() override;

	virtual void Done_Implementation(bool IsOk) override;
	virtual void DoReset_Implementation() override;
	
	UFUNCTION()
	void PlayDone();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USoundBase* SND_Wrong = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USoundBase* SND_Right = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USoundBase* SND_Reset = nullptr;

	UPROPERTY(Transient)
	USoundBase* SND = nullptr;
};

// TODO refactor all the sound stuff