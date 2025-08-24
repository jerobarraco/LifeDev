// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCheck.generated.h"

class UTextBlock;
class UCheckBox;
// basic checkbox
UCLASS()
class LIFEDEV_API ULCheck : public UUserWidget {
	GENERATED_BODY()
public:
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UCheckBox> Check = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Text = nullptr;
};
