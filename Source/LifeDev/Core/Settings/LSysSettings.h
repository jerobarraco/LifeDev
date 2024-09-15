#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"

class UDataTable;

// Make sure to update FeatUpdated() after changing the feats

// game features
UENUM(BlueprintType)
enum class EFeat: uint8 {
	NONE, // empty one to be able to add to the set.
	/// General

	// Chaps
	C_00,
	C_01,
	C_02,
	C_03,
	C_04,
	C_05,
	C_06,
	C_07,
	C_08,
	C_09,

#pragma region Dialogs
	// Shows the dialogs, otherwise they get muted
	D_ALL,
	// Show the text on the dialogs, otherwise they get censored
	D_TEXT,
#pragma endregion

#pragma region Sounds
	// plays music
	S_MUSIC,
	// plays the noises
	S_NOISE,
	// ambient sound
	S_ENV,
#pragma endregion

#pragma region Visuals
	// lumen gi
	V_LUMEN,
	// blur
	V_BLUR,
	// strobing lights
	V_STROBE,
	// field of view
	V_FOV,
	V_SPEED,
#pragma endregion
	
#pragma region Environ
	E_GHOSTPOOL,
#pragma endregion
	// Gameplay (find another word)
	// G_*,
	
	/// Debug
#pragma region Debug
	// Debug steps (cheats)
	DBG_STEPS,
	// debug the animator
	DBG_ANIMS,
	// debug sounds
	DBG_SOUND,
	// debug for flashbacks
	DBG_FB,
	// significance
	DBG_SIG,
	// General debug, deprecated. don't use unless you are me.
	DBG_ALL UMETA(Deprecated),
#pragma endregion
};

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

	// whether to use the feats listed in debugfeats or in defaultfeats
	// this is set to false on release builds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	bool UseDebugFeats = false;

	// if this is false it will always create a new savefile and ignore the saved one.
	// this is set to true on release builds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	bool UseSaveGame = true;

	// The default features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	TSet<EFeat> DefaultFeats = {
		EFeat::D_ALL, EFeat::D_TEXT, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_STROBE, EFeat::V_FOV, EFeat::V_LUMEN, EFeat::V_BLUR,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03,
	};

	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	TSet<EFeat> DebugFeats = {
		EFeat::DBG_STEPS, EFeat::DBG_ALL,
		EFeat::D_ALL, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_STROBE, EFeat::V_FOV, EFeat::V_LUMEN, EFeat::V_BLUR,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03,
	};
};