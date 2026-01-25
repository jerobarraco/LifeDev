// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"

#include "LFeatsMan.generated.h"

enum class EFeat : uint8;
class UInputMappingContext;
class UInputAction;
class ULSettingsUI;
class ULOverlayUI;
class ULSettings;
class UEval;
class ALGGameMode;
class AGhostPool;

// Handles feat changes and init for things that can't do it themselves ONLY
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALFeatsMan : public AInfo {
	GENERATED_BODY()

public:
	ALFeatsMan();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALFeatsMan* Instance(const UObject* const O);

	UFUNCTION(BlueprintCallable)
	void DoEffect(const FName& Name, const bool Enable);

	// Called by LGGameMode
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	virtual void Init();
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	virtual void DeInit();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULSettingsUI* GetSettingsUI() const { return SettingsUI; }

	// reapplies the blur
	UFUNCTION(BlueprintCallable)
	void BlurReset();
	// reapplies the color aberration
	UFUNCTION(BlueprintCallable)
	void FringeReset();

	static constexpr float DefFringe = 1;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float FringeIntensity = DefFringe;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurMax = 5;
	static constexpr float DefBlurAmount = .5;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurAmount = DefBlurAmount;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 ZOrder = 111;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<AGhostPool> Ghosts = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void MenuDone();
	void ActMenu();

#pragma region feats
	void LoadMPC();
	void LoadFeats();
	UFUNCTION()
	void FeatUpVisual(const EFeat Feat, const bool Enabled);
	UFUNCTION()
	void FeatUpUnreal(const EFeat Feat, const bool Enabled);
	UFUNCTION()
	void FeatUpGame(const EFeat Feat, const bool Enabled);
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	TSubclassOf<ULOverlayUI> OverlayUIClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<ULSettingsUI> SettingsUIClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	TObjectPtr<UInputAction> ActionMenu = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UInputMappingContext> Context = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 InputPrio = 3;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=SetUp)
	float ExposureFixed = 2;

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
