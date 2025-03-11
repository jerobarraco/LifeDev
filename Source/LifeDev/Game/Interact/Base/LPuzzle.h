// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "Interact/Puzzle.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LPuzzle.generated.h"

class UCLSounder;
class AInteract;
class UCPuzzle;
class UStory;
class UInventory;
class UFlags;
class UDiags;
class UFlashback;

// unfortunately unreal does not allow to have multiple inheritance
// and 
// Base class for LPuzzles
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALPuzzle: public APuzzle {
	GENERATED_BODY()

public:
	ALPuzzle();

	// sets the UseItemDlg map on each registered interact.
	// Use on PostLoad (or BeginPlay) (if you've set the interacts on the editor's world outliner
	// unless you've set the reference of the CPuzzle->Interacts on the constructor).
	UFUNCTION(BlueprintCallable)
	void SetUseItemDlgs(const TMap<FName, FName>& Map);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Done_Implementation(const bool IsOk) override;

	// Dialog to show on done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	FName DoneDlg = NAME_None;

	// Flag to reward on done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	FName DoneFlag = NAME_None;

	// Item to reward on done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	FName DoneItem = NAME_None;

	// Step to finish on done. If none it won't stop anything.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	FName DoneStep = NAME_None;

	// FB to reward on done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	float DoneFB = 0;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCLSounder> SND = nullptr;

	// caches
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlashback> FB = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UStory> Story = nullptr;
};
