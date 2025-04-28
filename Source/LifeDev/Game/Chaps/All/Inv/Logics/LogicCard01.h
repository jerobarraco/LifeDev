// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Inventory/Logics/LogicCard.h"

#include "LogicCard01.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FCard01Entry: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> From;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> To;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Consume = true;
};

// item logic for card 01
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULogicCard01: public ULogicCard {
	GENERATED_BODY()

public:
	ULogicCard01();
	virtual void Use_Implementation() override;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> DT = nullptr;
};
