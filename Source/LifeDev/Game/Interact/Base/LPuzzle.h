// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LPuzzle.generated.h"

class AInteract;
class UCPuzzle;
class UStory;
class UInventory;
class UFlags;
class UDiags;
class UFlashback;

// Base class for LPuzzles
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API ALPuzzle: public AActor {
	GENERATED_BODY()

public:
	ALPuzzle();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Done(bool IsOk);
	virtual void Done_Implementation(bool IsOk);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation() {};

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Dialog to show on done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DoneDlg = NAME_None;

	// Interact to trigger on Done. It will force unlock.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AInteract* Interact= nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=SetUp)
	UCPuzzle* CPuzzle = nullptr;
	UPROPERTY(BlueprintReadOnly)
	USceneComponent* Root = nullptr;

	// caches
	UPROPERTY(BlueprintReadOnly, Transient)
	UFlashback* FB = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UDiags* Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UFlags* Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UStory* Story = nullptr;
	
};

// TODO "Just think about it"
// the other option is to have a class that inherit from ALInteract like InteractSpot
// cons:
//		non-visible things will need the whole alinteract stuff (eg unlocking steps, dialogs, and fb)
//			interacts are quite too big at the moment.
//		quite potentially i will mud the interact logic
//			some flags could conflict (like locked and trigger)
//			ill have to take care of many more special cases
//			the interface becomes (too populated) uncomfortable to manage
// pros:
//		i don't need to clone a lot of code (subsystem caching)
//		visual LPuzzles will need an interact
//		less code to interface between the LPuzzle item an the visual interact (eg changing states and locks and stuff)

// not inheriting from APuzzle since that class doesn't really add much.
// and having another parent might hit performance a bit,
// more importantly it will limit how and what i do.
// the other class is better off as a sample for the plugin.