// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"

#include "InventoryManager.generated.h"

class UInventory;
class UInputMappingContext;
class UInputAction;
class UInventoryUI;

// base class for the character
UCLASS(Blueprintable, config=Game)
class INVENTORY_API AInventoryManager : public AActor {
	GENERATED_BODY()

public:
	AInventoryManager();
	
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void DeInit();
	
	UFUNCTION(BlueprintCallable)
	void Show();

	UFUNCTION(BlueprintCallable)
	void Hide();
	
	// void HideUI() const;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SetUp)
	int32 InputPrio = 9;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UUserWidget> UIClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputMappingContext* Mapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* ActionOpen = nullptr;

private:
	// stub. the manager will handle input. will it?
	UFUNCTION()
	void UIDone();
	
	UPROPERTY(Transient)
	UInventory* Inventory = nullptr;

	UPROPERTY(Transient)
	UInventoryUI* UI = nullptr;

	bool IsShowing = false;
};