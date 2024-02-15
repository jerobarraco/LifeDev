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
	UFUNCTION()
	void PlayDone();
	UFUNCTION()
	void Doned(bool bIsOn);
	
	virtual void PostLoad() override;
	// virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USoundBase* SND_Wrong = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USoundBase* SND_Right = nullptr;

	UPROPERTY(Transient)
	USoundBase* SND = nullptr;
};


// TODO refactor all the sound stuff