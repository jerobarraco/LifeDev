// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "InteractorUI.generated.h"

// Helps define an interaction volume
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UInteractorUI: public UUserWidget {
public:
	GENERATED_BODY()

	
	UFUNCTION(BlueprintNativeEvent)
	void SetPrompt(const FText& text);
};
