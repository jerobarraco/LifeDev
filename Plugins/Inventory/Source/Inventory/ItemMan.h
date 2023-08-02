#pragma once

#include "CoreMinimal.h"

#include "ItemMan.generated.h"

// Base class for the manager of each item 
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Inventory))
class INVENTORY_API UItemMan: public UObject {
public:
	GENERATED_BODY()

	UItemMan();

	UFUNCTION(BlueprintCallable)
	void Hide() {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Use();
	void Use_Implementation() {
		UE_LOG(LogTemp, Warning, TEXT("Using item %llu"), (int64)this);
	};
	
protected:
	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	// TSubclassOf<UItemViewUI> UIClass = nullptr;
};
