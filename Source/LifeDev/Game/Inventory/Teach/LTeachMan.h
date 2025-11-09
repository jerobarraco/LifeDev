// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Inventory/Teach/TeachMan.h"

#include "LTeachMan.generated.h"

enum class EFeat : uint8;
struct FDiag;
struct FItem;
class AStep;
class ULSettingsUI;
class ULSettings;
class UStory;
class UDiags;
class UInventory;
class UCInteract;

UCLASS(Blueprintable, BlueprintType, Config=Inventory, DefaultConfig)
class LIFEDEV_API ALTeachMan: public ATeachMan {
	GENERATED_BODY()
public:
#pragma region base
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALTeachMan* Instance(const UObject* const O);
	virtual void Init_Implementation() override;
	virtual void DeInit_Implementation() override;
	UFUNCTION()
	void InitDelayed(); // called when the game already started

	// will set the key names to whatever is current in the imcs
	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetCurrentKeyNames();
#pragma endregion

	// time to wait before triggering the delayed init
	UPROPERTY(BlueprintReadWrite, Config)
	float InitDelayTime = 5;
#pragma region delegates
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual bool Show_Implementation(const FName& Id) override;
	void DeInitItemMod() const;
	void DeInitInter();
	void DeInitLook();
	void DeInitStory();
	void InitFeat();
	void DeInitFeat();
	UFUNCTION()
	void ItemMod(const FName& Name, const int32 Diff, const FItem& Item);
	UFUNCTION()
	void InterTrigger(const UCInteract* const Comp);
	UFUNCTION()
	void InterHover(const bool bOn, UCInteract* const Comp);
	UFUNCTION()
	void ItemSel(const FName& Name);
	UFUNCTION()
	void ItemUse(const FName& Name);
	bool ItemHasAll() const;
	UFUNCTION()
	void ItemLook(const FName& Name);
	UFUNCTION()
	void StepStart(AStep* const Step);
	UFUNCTION()
	void FeatUp(const EFeat Feat, const bool Enabled);
	void FlashTeach();
	UFUNCTION()
	void FlagHide(const FName& Id);
	void FlashDone();
	// only call after closing settings
	UFUNCTION()
	void SettingsDone();

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UStory> Story = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettings> Settings = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettingsUI> SettingsUI = nullptr; 
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Items = nullptr;

	FTimerHandle HFlash;

	uint8 ItemSelCount = 0;
};
