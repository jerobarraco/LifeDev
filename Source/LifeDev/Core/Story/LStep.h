// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryTypes.h"
#include "Story/Step.h"

#include "LStep.generated.h"

class UDialogs;
class UInventory;

UCLASS(Blueprintable, BlueprintType)
class ALStep : public AStep {
	GENERATED_BODY()

public:
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;

	// whether to dis/enable the character input
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool InputEnabled = false;

	// when set to true, the game mode will set the wait time to the fade time. Also check FinishPostWait
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool UseFadeTime = false;

	// dialog or sequence to trigger on start. This will make the step finish when the dialog finishes.
	// it will also disable FinishPostWait
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DlgId = NAME_None;

	// if this is set. it will advance once ALL items are obtained.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> FinishItems;

	// the game fade time, please set this from outside.
	inline static float FadeTime = 0;

protected:
	// will get called when DEBUG_STEPS flag is set
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Debug();
	virtual void Debug_Implementation();
	
	virtual void PostWait_Implementation() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;

	
	// will start the dialogs
	void StartDialogs();

	UFUNCTION()// bound
	void ItemMod(const FName& ItemName, int32 Diff, const FItem& Item);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UDialogs* Dialogs = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UInventory* Inventory = nullptr;
};
