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
	TObjectPtr<AInteractAnim> Lid = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TObjectPtr<USoundBase> SND_Wrong = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TObjectPtr<USoundBase> SND_Right = nullptr;

	bool WasOk = false;
	uint8 FailDiagIndex = 0;
	TArray<FName> FailDiags = {
		// notice this is the last one. so that the code can be a bit lazier but still correct.
		"PZ04xC02",
		"PZ04xC00",
		"PZ04xC01",
	};
	// reusing the dialogs from the card is fine atm.
	// the whole reason of this feature is JUST IN CASE the player
	// didn't figure out that it needed to use the cards
	// (because it's an obscure mechanic that is not used elsewhere atm).
};
