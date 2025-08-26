// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI04.generated.h"

class AInteractAnim;

// Chap 03 Puzzle Instance 04 Piano 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI04: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI04();

protected:
	virtual void BeginPlay() override;
	virtual void Done_Implementation(const bool Ok) override;

	UFUNCTION()
	void PostDone();
	UFUNCTION()
	void PostDoneSnd();
	UFUNCTION()
	void LidDone();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TObjectPtr<AInteractAnim> Lid = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TObjectPtr<USoundBase> SND_Wrong = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TObjectPtr<USoundBase> SND_Right = nullptr;

	bool WasOk = false;
};
