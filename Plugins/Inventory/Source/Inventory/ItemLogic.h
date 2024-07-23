#pragma once

#include "CoreMinimal.h"

#include "ItemLogic.generated.h"

// Base class for the logic of each item 
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Inventory))
class INVENTORY_API UItemLogic: public UObject {
	GENERATED_BODY()

public:
	UItemLogic();

	// the world passed by parameter is trash. don't use.
	// warning: this function can sometimes be called during editor time.
	// set an appropriate outer when creating this object and use GetWorld() when needed.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginPlay(UWorld* BrokenDontUse);
	virtual void BeginPlay_Implementation(UWorld* BrokenDontUse) {}
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Look();
	virtual void Look_Implementation() {
		UE_LOG(LogTemp, Log, TEXT("ItemLogic.Look: %lu %s"), (int64)this, *Name.ToString());
	}

	// this will get triggered only for self-use
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Use();
	virtual void Use_Implementation() {
		UE_LOG(LogTemp, Log, TEXT("ItemLogic.Use: *%lu. '%s'"), (int64)this, *Name.ToString());
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	FName Name;

protected:
	// I can't save the world here since it will crash the editor on load for some weird reason
};
