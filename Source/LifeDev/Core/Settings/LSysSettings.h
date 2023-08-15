#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"

class UDataTable;

// game feature
UENUM(BlueprintType)
enum class EFeat: uint8 {
	NONE, // empty one to be able to add to the set.
	// Debug steps (cheats)
	DEBUG_STEPS,
	// debug the animator
	DEBUG_ANIMATOR,
	// Shows the dialogs, otherwise they get muted
	DIALOGS,
	// Show the text on the dialogs, otherwise they get censored
	DIALOG_TEXT,
	MUSIC,
	// chaps
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
	
	// INVENTORY,
	// DIALOGS,

	// General debug, deprecated
	DEBUG,
};

namespace LifeDev {
	namespace Feats {
		static EFeat ChapFeats[] = {
			EFeat::CHAP_00, EFeat::CHAP_01, EFeat::CHAP_02, EFeat::CHAP_03, EFeat::CHAP_04,
			EFeat::CHAP_05, EFeat::CHAP_06, EFeat::CHAP_07, EFeat::CHAP_08, EFeat::CHAP_09};
		static uint8 ChapFeatN = 10;
	}
}


// Note the Config meta tag on the properties are critical or it might crash
// Settings for the game (to be built)
UCLASS(Config=LDSettings, defaultconfig, meta=(DisplayName="LifeDevSystemSettings"))
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
	TSoftObjectPtr<UDataTable> Chapters;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	bool UseDebugFeats = false;
	// The default features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	TSet<EFeat> DefaultFeats = {EFeat::CHAP_00, EFeat::CHAP_01};
	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	TSet<EFeat> DebugFeats = {EFeat::DEBUG_STEPS, EFeat::CHAP_01};
};