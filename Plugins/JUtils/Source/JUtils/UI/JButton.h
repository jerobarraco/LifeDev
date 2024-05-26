// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "JButton.generated.h"

class UButton;
class UTextBlock;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJButtonClick, int32, Id);

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UJButton: public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DoClick();

	UFUNCTION(BlueprintCallable)
	void SetUp(const FText& NewText, const int32 NewId = -1);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 Id=-1;

	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FJButtonClick OnClick;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* Text = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn = nullptr;
};
