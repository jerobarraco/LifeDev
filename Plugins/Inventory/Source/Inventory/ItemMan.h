#pragma once

#include "CoreMinimal.h"

#include "ItemMan.generated.h"

// Base class for the manager of each item 
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Inventory))
class INVENTORY_API UItemMan: public UObject {
public:
	GENERATED_BODY()

	UItemMan();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Look();
	void Look_Implementation() {
		UE_LOG(LogTemp, Log, TEXT("ItemMan.Look. Looking item %lu %s"), (int64)this, *Name.ToString());
	};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Use();
	void Use_Implementation() {
		UE_LOG(LogTemp, Log, TEXT("ItemMan.Use: Using item %lu. '%s'"), (int64)this, *Name.ToString());
	};

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FName Name;
};
