// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "Inventory/ItemLogic.h"

#include "LItemLogic.generated.h"

class UFlashback;
class UDiags;

// base class for all item logics in LifeDev
// (that interacts with all the other plugins)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULItemLogic: public UItemLogic {
	GENERATED_BODY()

public:
	ULItemLogic();
	virtual void Look_Implementation() override {}
	virtual void Use_Implementation() override;
	
protected:
	virtual void BeginPlay_Implementation(UWorld* NewWorld) override;
	virtual void BeginDestroy() override;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UDiags* Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UFlashback* FB = nullptr;
};
