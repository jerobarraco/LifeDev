#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"

class UDataTable;


// game feature
UENUM(BlueprintType)
enum class EFeat {
	DUNNO,
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Dialogs", meta=(RowType="/Script/Dialogs.DialogChar"))
	TSoftObjectPtr<UDataTable> Characters = nullptr;
	// The dialogs for each chapter
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Dialogs", meta=(RowType="/Script/Dialogs.Dialog"))
	TArray<TSoftObjectPtr<UDataTable>> ChapDialogs;
	// The list of sequences
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Dialogs", meta=(RowType= "/Script/Dialogs.DialogChar"))
	TArray<TSoftObjectPtr<UDataTable>> Sequences;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	bool UseDebugFeats = false;

	// The default features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	TSet<EFeat> DefaultFeats = {EFeat::DUNNO};

	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Core")
	TSet<EFeat> DebugFeats = {EFeat::DUNNO};
	
};