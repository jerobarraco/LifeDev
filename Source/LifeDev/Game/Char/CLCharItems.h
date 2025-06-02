// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Interact/InteractTypes.h"
#include "Inventory/InventoryTypes.h"

#include "CLCharItems.generated.h"

class UFlags;
class UCInteractor;
class UInventory;
class UDiags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLook, )

// LifeDev Game character items
// handles some stuff regarding items... 
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API UCLCharItems: public UActorComponent {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool Say(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	void Look(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	EItemUseResult Use(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	EItemUseResult UseSelected() const;
	UFUNCTION(BlueprintCallable)
	void LookSelected() const;
	UFUNCTION(BlueprintCallable)
	bool PlaySound(const TSoftObjectPtr<USoundBase>& Snd) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config)
	bool UseSndAtLocation = false;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	bool DoUse(const FName Name, const FItem& Item) const;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UCInteractor> Interactor = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags=nullptr;
};
