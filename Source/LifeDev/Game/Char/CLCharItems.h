// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "CLCharItems.generated.h"

class UCInteractor;
class UInventory;
class UDiags;

// LifeDev Game character items
// handles some stuff regarding items... 
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API UCLCharItems : public UActorComponent {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool Say(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	void Look(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	void Use(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	void UseSelected() const;
	UFUNCTION(BlueprintCallable)
	void LookSelected() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, Transient)
	UDiags* Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UCInteractor* Interactor = nullptr;
};
