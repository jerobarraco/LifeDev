// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GroupBox.h"

#include "LSaveGroup.generated.h"

class UJButton;
class USlider;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSGOnLoadDone, const bool, HasDoneSave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSGOnDone, const int32, Id);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSGOnSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSGOnTryErase);

// LD basic savefile group box
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSaveGroup : public UGroupBox {
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void LoadDoneAll();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DoErase();
	virtual void DoErase_Implementation(){} // TODO move here


	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Transient)
	FSGOnLoadDone OnLoadDone;
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Transient)
	FSGOnDone OnDone;
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Transient)
	FSGOnSettings OnSettings;
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, Transient)
	FSGOnSettings OnTryErase;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void FoxyUpd(const float Value);
	UFUNCTION()
	void DoStart(const int32 pId);
	UFUNCTION()
	void DoSettings(const int32 pId);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<USlider> SLFoxy = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> TFoxy = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnSettings = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnStart = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient)
	bool HasDoneSave = false;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool LoadAllSlots = false;
};
