#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LSysSettings.generated.h"

class UDataTable;

// Make sure to update FeatUpdated() after changing the feats

// this class is for the settings that appears on the editor under project settings. The general system settings.
// For the in game settings that change during gameplay (transient ones) see LSettings
// Note: the Config meta tag on the properties is critical, or it might crash.
// Settings for the game (to be built)
// the config file is available after shipping at ~/.config/Epic/{Project}/Saved/Config/{Linux|Windows} or C:\Users\{UserName}\AppData\Local\{ProjectName}\Saved\Config\{Linux|Windows}
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig, meta=(DisplayName="LifeDevSystemSettings"))
class LIFEDEV_API ULSysSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static ULSysSettings* Get();

	// Overrides for display
	virtual FName GetCategoryName() const override;

	// returns initial features for shipping builds (Default on shipping, debug if debug enabled and debug build)
	// for the *actual current instance* feats, get them from LSettings
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSet<EFeat>& GetFeats();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetUseDebugFeats() const;

	// The list of items
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Inventory",
		meta=(RowType="/Script/Inventory.Item"))
	TSoftObjectPtr<UDataTable> Inventory = TSoftObjectPtr<UDataTable>(
		FSoftObjectPath("/Game/LifeDev/Game/Inventory/DT_Inventory"));

	// The list of characters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(RowType="/Script/Dialogs.DialogChar"))
	TSoftObjectPtr<UDataTable> Characters = TSoftObjectPtr<UDataTable>(
		FSoftObjectPath("/Game/LifeDev/Game/Dialogs/DT_DiagChars"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(RowType="/Script/LifeDev.LChapter"))
	TSoftObjectPtr<UDataTable> Chapters =
		TSoftObjectPtr<UDataTable>(FSoftObjectPath("/Game/LifeDev/Game/Sys/DT_Chapters"));

	// the chapter to start with
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(ClampMax=4, ClampMin=0, UIMin=0, UIMax=4))
	int32 StartChap = -1;


	// The default features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	TSet<EFeat> DefaultFeats = {
		EFeat::D_SHOW, EFeat::D_TEXT,
		EFeat::S_ENV, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_FOV, EFeat::V_BLUR, EFeat::V_LUMEN, EFeat::V_MLIGHTS, EFeat::V_NANITE,
		EFeat::V_SPEED, EFeat::V_FLASHBACK, EFeat::V_STROBE,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03,
	};

	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	TSet<EFeat> DebugFeats = {
		EFeat::DBG_STEPS, EFeat::DBG_BASE,
		EFeat::D_SHOW,
		EFeat::S_ENV, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_FOV, EFeat::V_BLUR, EFeat::V_LUMEN, EFeat::V_MLIGHTS, EFeat::V_NANITE,
		EFeat::V_SPEED, EFeat::V_FLASHBACK, EFeat::V_STROBE,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03,
	};
protected:
	// whether to use the feats listed in debugfeats or in defaultfeats
	// this is set to false on release builds.
	// only toggle this on the editor preferences, but during runtime call GetUseDebugFeatsS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	bool UseDebugFeats = false;
};