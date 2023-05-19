#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"

class UDataTable;

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

	// The dialogs for each chapter
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, AdvancedDisplay, Category="Dialogs", meta=(RowType="Dialog"))
	TArray<TSoftObjectPtr<UDataTable>> ChapDialogs;
	// The list of characters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, AdvancedDisplay, Category="Dialogs", meta=(RowType="DialogChar"))
	TSoftObjectPtr<UDataTable> Characters = nullptr;
	// The list of sequences
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, AdvancedDisplay, Category="Dialogs", meta=(RowType="DialogChar"))
	TSoftObjectPtr<UDataTable> Sequences = nullptr;
};