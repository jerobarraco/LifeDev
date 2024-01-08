// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Story/Story.h"

#include "Puzzle.generated.h"

// TODO "Just think about it"
// the other option is to have a class that inherit from ALInteract like InteractSpot
// cons:
//		non-visible things will need the whole alinteract stuff (eg unlocking steps, dialogs, and fb)
//			interacts are quite too big at the moment.
//		quite potentially i will mud the interact logic
// pros:
//		i don't need to clone a lot of code (subsystem caching)
//		visual puzzles will need an interact
//		less code to interface between the puzzle item an the visual interact (eg changing states and locks and stuff)


class UFlags;
class UInventory;
class UDiags;
class UFlashback;
class UCPuzzle;

// Base class for Puzzles
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzle: public AInfo {
	GENERATED_BODY()

public:
	APuzzle();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Done(bool IsOk);
	virtual void Done_Implementation(bool IsOk) {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation() {};
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=SetUp)
	UCPuzzle* CPuzzle = nullptr;

	// caches
	UPROPERTY(Transient)
	UFlashback* FB = nullptr;
	UPROPERTY(Transient)
	UDiags* Diags = nullptr;
	UPROPERTY(Transient)
	UFlags* Flags = nullptr;
	UPROPERTY(Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(Transient)
	UStory* Story = nullptr;
};
