// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InventoryTypes.h"

#include "InventoryMan.generated.h"

class UInventory;
class UInputMappingContext;
class UInputAction;
class UInventoryUI;
class UCItemView;

// base class for the character
UCLASS(Blueprintable)
class INVENTORY_API AInventoryMan : public AInfo {
	GENERATED_BODY()

public:
	AInventoryMan();
	
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void DeInit();

	UFUNCTION(BlueprintCallable)
	void SetVisible(bool Vis);

	UFUNCTION(BlueprintCallable)
	void Show();
	
	UFUNCTION(BlueprintCallable)
	void Hide();

	UFUNCTION(BlueprintCallable)
	void SetSelected(const FName& Name);
	UFUNCTION(BlueprintCallable)
	void SetItemMod(const FName& Name, int32 Diff, const FItem& Item);
	UFUNCTION(BlueprintCallable)
	void SetItemCold(const FName& Name);
	UFUNCTION(BlueprintCallable)
	void SetItemUsed(const FName& Name);

	UFUNCTION()
	void ActOpen();
	UFUNCTION()
	void ActSelect(const FInputActionValue& InputActionValue);

	// void HideUI() const;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SetUp)
	int32 InputPrio = 9;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 ZOrder = 1;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UInventoryUI> UIClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UInputMappingContext> Mapping = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UInputAction> ActionOpen = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UInputAction> ActionSelect = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCItemView> View = nullptr;

private:
	// stub. the manager will handle input. will it?
	UFUNCTION()
	void UIDone();

	UPROPERTY(Transient)
	TObjectPtr<UInventory> Inventory = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UInventoryUI> UI = nullptr;

	bool IsShowing = false;
};