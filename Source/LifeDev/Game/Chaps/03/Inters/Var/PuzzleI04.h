// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI04.generated.h"

class AInteractAnim;

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

	UFUNCTION()
	void PostDone();
	UFUNCTION()
	void PostDoneSnd();
	UFUNCTION()
	void LidDone();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	AInteractAnim* Lid = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USoundBase* SND_Wrong = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USoundBase* SND_Right = nullptr;

	bool WasOk = false;
};

// TODO refactor all the sound stuff