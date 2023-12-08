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
	CHAP_00,
	CHAP_01,
	CHAP_02,
	CHAP_03,
	CHAP_04,
	CHAP_05,
	CHAP_06,
	CHAP_07,
	CHAP_08,
	CHAP_09,
	// Shows the dialogs, otherwise they get muted
	DIALOGS,
	// Show the text on the dialogs, otherwise they get censored
	DIALOG_TEXT,
	// plays music
	MUSIC,

	/// Access
	// strobing lights
	A_STROBE,

	// Graphic
	G_LUMEN,
	G_BLUR,
	/// Debug
	// Debug steps (cheats)
	DEBUG_STEPS,
	// debug the animator
	DEBUG_ANIMS,
	// General debug, deprecated. don't use unless you are me.
	DEBUG UMETA(Deprecated),
};

// this class is for the settings that appears on the editor under project settings
// Note the Config meta tag on the properties are critical or it might crash
// Settings for the game (to be built)
UCLASS(Blueprintable, Config=LSysSettings, meta=(DisplayName="LifeDevSystemSettings"))
class LIFEDEV_API ULSysSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static ULSysSettings* Get();
	// Overrides for display
	virtual FName GetCategoryName() const override;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	bool UseDebugFeats = false;

	// The default features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	TSet<EFeat> DefaultFeats = {EFeat::DIALOGS, EFeat::DIALOG_TEXT, EFeat::MUSIC,
		EFeat::CHAP_00, EFeat::CHAP_01, EFeat::CHAP_02,
		EFeat::A_STROBE
	};

	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	TSet<EFeat> DebugFeats = {EFeat::DEBUG_STEPS, EFeat::DEBUG, EFeat::MUSIC,
		EFeat::CHAP_00, EFeat::CHAP_01, EFeat::CHAP_02,
		EFeat::A_STROBE
	};
};