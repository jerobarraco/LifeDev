// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"

#include "LFeatsMan.generated.h"

class UInputAction;
class ULSettingsUI;
enum class EFeat : uint8;
class ULOverlayUI;
class ULSettings;
class UEval;
class ALGGameMode;

// Handles feat changes and init for things that can't do it themselves ONLY
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALFeatsMan : public AInfo {
	GENERATED_BODY()

public:
	ALFeatsMan();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALFeatsMan* Instance(const UObject* const O);

	// Called by LGGameMode
	UFUNCTION(meta=(AdvancedDisplay))
	virtual void Init();

	// reapplies the blur
	UFUNCTION(BlueprintCallable)
	void BlurReset();

	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float FringeIntensity = 1;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurMax = 5;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurAmount = .5;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 ZOrder = 111;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void MenuDone();
	UFUNCTION()
	void ActMenu();

#pragma region feats
	void LoadMPC();
	void LoadFeats();
	UFUNCTION()
	void FeatUpVisual(const EFeat Feat, const bool Enabled);
	UFUNCTION()
	void FeatUpUnreal(const EFeat Feat, const bool Enabled);
	UFUNCTION()
	void FeatUpDbg(const EFeat Feat, const bool Enabled);
#pragma endregion

	UFUNCTION()
	double GetVar(const FName& Name);
	UFUNCTION()
	void SetVar(const FString& Name, const double Val);
	UFUNCTION()
	void SetVarId(const double NameID, const double Val);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UMaterialParameterCollection> MPC = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UDataLayerAsset> TestDL = TSoftObjectPtr<UDataLayerAsset> (
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Test.Test"));

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config)
	TSubclassOf<ULOverlayUI> OverlayUIClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<ULSettingsUI> SettingsUIClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	TObjectPtr<UInputAction> ActionMenu = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettingsUI> SettingsUI = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ULOverlayUI> OverlayUI = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCI = nullptr;
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInterface> SpeedMat = nullptr;
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInterface> FBMat = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ALGGameMode> GM = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UEval> Eval = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettings> Settings = nullptr;
};
