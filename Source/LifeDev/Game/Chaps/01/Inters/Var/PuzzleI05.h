// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI05.generated.h"

class ALStep;

// Chap 01 Puzzle Instance 01 fight/anger 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI05: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI05();

protected:
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	void Done_Implementation(const bool IsOk) override;

	UFUNCTION(CallInEditor, BlueprintCallable)
	void Danger(const int64 A);
	
	static void (ALStep::*fp)();
};
