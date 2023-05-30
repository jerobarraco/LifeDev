#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"

class UDataTable;


// game feature
UENUM(BlueprintType)
enum class EFeat {
	DUNNO,
	CHAP_00,
	// INVENTORY,
	// DIALOGS,
};

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
	TSet<EFeat> DefaultFeats = {EFeat::CHAP_00};
	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	TSet<EFeat> DebugFeats = {EFeat::CHAP_00};
	
};