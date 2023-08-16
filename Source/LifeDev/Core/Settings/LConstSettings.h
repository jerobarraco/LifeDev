#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.h"
#include "LConstSettings.generated.h"

class UDataTable;

// TODO This is a test and it might get removed soon. i'm evaluating if this is needed or not.
// depends on whether i will need this values on bps or not. and how expensive it is to call from cpp, on how valuable is to have compile time checks
// - It looks like a lot of overhead if you ask me.

USTRUCT(Blueprintable, BlueprintType)
struct FConstItemNames {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Config, Category="Items")
	FName Card0 = FName("C0");
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Config, Category="Items")
	FName Poem0 = FName("P00");
};
// Note the Config meta tag on the properties are critical or it might crash
// Settings for the consts in the game. not intended to be changed. but to be accessed across the game.
UCLASS(Blueprintable, Config=LConstSettings, defaultconfig, meta=(DisplayName="LifeDevConstSettings"))
class LIFEDEV_API ULConstSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static ULConstSettings* Get();
	// Overrides for display
	virtual FName GetCategoryName() const override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Config, Category="Chaps")
	TArray<EFeat> ChapFeats = {
		EFeat::CHAP_00, EFeat::CHAP_01, EFeat::CHAP_02, EFeat::CHAP_03, EFeat::CHAP_04,
		EFeat::CHAP_05, EFeat::CHAP_06, EFeat::CHAP_07, EFeat::CHAP_08, EFeat::CHAP_09};

	// item names
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Config, Category="Items")
	FConstItemNames MyStruct;
};
