#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"

class UDataTable;

UCLASS(Config=LifeDevSysSettings, DefaultConfig, meta=(DisplayName="LifeDev System Settings"))
class LIFEDEV_API ULSysSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static ULSysSettings* Get();
	// Overrides for display
	virtual FName GetCategoryName() const override;

	// The dialogs for each chapter
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	TArray<TSoftObjectPtr<UDataTable>> ChapDialogs;
};