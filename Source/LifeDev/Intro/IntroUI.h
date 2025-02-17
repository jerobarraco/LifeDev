// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "IntroUI.generated.h"

class UMsgBox;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIntroUIDone);

UCLASS(Blueprintable)
class LIFEDEV_API UIntroUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Done() {OnDone.Broadcast();}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void ShowMsg(const FText& Msg); // unused

	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FIntroUIDone OnDone;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void SlotLoadDone(const bool HasDoneSave);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UMsgBox> MsgBox = nullptr;
};
