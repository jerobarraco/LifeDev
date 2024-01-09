// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Puzzle.generated.h"

class AInteract;
class UCPuzzle;

// Base class for Puzzles that can optionally trigger an interact
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API APuzzle: public AInfo {
	GENERATED_BODY()

public:
	APuzzle();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Done(bool IsOk);
	virtual void Done_Implementation(bool IsOk);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation() {};

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	AInteract* Interact = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCPuzzle* CPuzzle = nullptr;
};
