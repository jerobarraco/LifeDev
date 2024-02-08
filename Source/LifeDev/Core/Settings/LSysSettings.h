#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"

class UDataTable;

// game feature
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

	/// Dialogs
	
	// Shows the dialogs, otherwise they get muted
	D_ALL,
	// Show the text on the dialogs, otherwise they get censored
	D_TEXT,

	/// Sound
	
	// plays music
	S_MUSIC,
	// plays the noises
	S_NOISE,
	// ambient sound
	S_ENV,

	/// Graphic
	
	V_LUMEN,
	V_BLUR,

	/// Access
	// strobing lights
	A_STROBE,

	// Gameplay (find another word)
	// G_*,
	
	/// Debug
	
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
};

// this class is for the settings that appears on the editor under project settings. The general system settings.
// For the in game settings that change during gameplay (transient ones) see LSettings
// Note the Config meta tag on the properties are critical or it might crash
// Settings for the game (to be built)
// the config file is available after shipping at ~/.config/Epic/ or C:\Users\{UserName}\AppData\Local\{ProjectName}
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig, meta=(DisplayName="LifeDevSystemSettings"))
class LIFEDEV_API ULSysSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	static ULSysSettings* Get();
	// Overrides for display
	virtual FName GetCategoryName() const override;

	// returns initial features for shipping builds (Default on shipping, debug if debug enabled and debug build)
	// for the *actual current instance* feats, get them from LSettings
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSet<EFeat>& GetFeats();
	
	// The list of items
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Inventory", meta=(RowType="/Script/Inventory.Item"))
	TSoftObjectPtr<UDataTable> Inventory = nullptr;

	// The list of characters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story", meta=(RowType="/Script/Dialogs.DialogChar"))
	TSoftObjectPtr<UDataTable> Characters = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story", meta=(RowType="LChapter"))
	TSoftObjectPtr<UDataTable> Chapters = nullptr;

	// the chapter to start with
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story")
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
		EFeat::A_STROBE, EFeat::V_LUMEN, EFeat::V_BLUR,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03,
	};

	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	TSet<EFeat> DebugFeats = {EFeat::DBG_STEPS, EFeat::DBG_ALL,
		EFeat::D_ALL, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::A_STROBE, EFeat::V_LUMEN, EFeat::V_BLUR,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03,
	};
};