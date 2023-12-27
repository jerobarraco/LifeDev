// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Story/Step.h"
#include "Inventory/InventoryTypes.h"

#include "LStep.generated.h"

class UFlashback;
class UDiags;
class UInventory;

// TODO make child classes use FB instead of flashback

// LifeDev base class for story steps
// be careful it will clear all timers on stop
UCLASS(Blueprintable, BlueprintType)
class ALStep : public AStep {
	GENERATED_BODY()

public:
	virtual void Start_Implementation() override;
	// careful this will clear all timers
	// and remove all delegates to Dialogs.OnDone and Inventory.OnMOd
	// it will also remove from inventory the items on RemItems
	virtual void Stop_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void CheckFinishItems();
	
	// whether to dis/enable the character input
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool InputEnabled = false;

	// when set to true, the game mode will set the wait time to the fade time.
	// See WaitTime and FinishPostWait
	// this is used to start something AFTER it faded
	// don't use for fade out, since waittime doesn't work with that, Stop is called just when the story faded out.
	// TODO make sure i dont need it and remove
	// TODO need to fix the steps that do use postwait
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp, meta=(DeprecatedProperty))
	bool UseFadeTime = false;
	
	// dialog or sequence to trigger on start. This will make the step finish when the dialog finishes.
	// it will also disable FinishPostWait
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DlgId = NAME_None;

	// if this is set. it will advance once ALL items are obtained.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> FinishItems;

	// items to remove when the step finishes
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> RemItems;

	// items to ensure when starting. mostly for skipping chapters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> ItemsEnsure;

	// TODO add EnsureItems to ensure you have the items by the chapter

	// TODO rename these Items vars
	// the game fade time, please set this from outside.
	inline static float FadeTime = 0;
	inline static bool UseDebug = false;
	
protected:
	virtual void PostWait_Implementation() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;
	virtual void Finish_Implementation() override;
	
	// will start the dialogs
	void StartDialogs();
	// removes items on RemItems
	void RemoveItems();
	void EnsureItems();
	
	UFUNCTION()// bound
	void ItemMod(const FName& ItemName, int32 Diff, const FItem& Item);

	UPROPERTY(BlueprintReadOnly, Transient)
	UDiags* Dialogs = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UFlashback* FB = nullptr;
};
