// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "Inventory/ItemLogic.h"

#include "LLogic.generated.h"

class UFlags;
class UInventory;
class UFlashback;
class UDiags;
class UStory;

// base class for all item logics in LifeDev
// (that interacts with all the other plugins)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULLogic: public UItemLogic {
	GENERATED_BODY()

public:
	ULLogic();

	// dialog to trigger on use
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName UseDlg = NAME_None;

protected:
	virtual void BeginPlay_Implementation() override;
	virtual void BeginDestroy() override;
	virtual void Use_Implementation() override;

	// will add a dialog or sequence. returns success. checks isvalid diags
	UFUNCTION(BlueprintCallable)
	bool Say(const FName& Id) const;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlashback> FB = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UStory> Story = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Inv = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
};
