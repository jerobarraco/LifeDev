#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ItemViewUI.generated.h"
UCLASS(Blueprintable)
class INVENTORY_API UItemViewUI: public UUserWidget {
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintNativeEvent)
	void Show();
	virtual void Show_Implementation() {};
	
	UFUNCTION(BlueprintNativeEvent)
	void Hide();
	virtual void Hide_Implementation() {};
};
