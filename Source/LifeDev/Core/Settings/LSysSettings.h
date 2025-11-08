#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Inventory/Teach/TeachTypes.h"
#include "JUtils/Misc/JUtilsSys.h"
#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LSysSettings.generated.h"

enum class ETeachTarget : uint8;
class UDataTable;

// Demo mode. avoid using the macro unless necessary.
#ifndef LD_DEMO
#define LD_DEMO 0
#endif

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
	static FORCEINLINE ULSysSettings* Get() {
		return GetMutableDefault<ULSysSettings>(); // the same as the previous code.
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE FName GetGameLvl() {
		static const FName G("Game_L");
		return G; // force game level
		// static const FName D("Demo_L");
		// #if LD_DEMO
		// return D;
		// #else
		// return G;
		// #endif
	}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE bool IsDemo() {
#if LD_DEMO
		return true;
#else
		return false;
#endif
	}

	// Overrides for display
	virtual FName GetCategoryName() const override{ 
		static const FName Cat = FName("LifeDev"); 
		return Cat;
	}

	// returns initial features for shipping builds (Default on shipping, debug if debug enabled and debug build)
	// for the *actual current instance* feats, get them from LSettings
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FORCEINLINE TSet<EFeat>& GetFeats() const {
		const bool UseDebug = GetUseDebugFeats();
		return UseDebug ? DebugFeats : DefaultFeats; // force defaults on release
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetUseDebugFeats() const {
		return UseDebugFeats & UJUtilsSys::IsDebug();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetStartChap() const{
		const bool UseDebug = GetUseDebugFeats();
		return UseDebug ? StartChap : 0; // force to 0 on release
	}

	// url to open when the game closes. usually the feedback form.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Sys")
	FString CloseURL = TEXT("https://forms.gle/aYCr8zRR3wWTomyu8");

	// The list of items
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Inventory",
		meta=(RequiredAssetDataTags="RowStructure=/Script/Inventory.Item"))
	TSoftObjectPtr<UDataTable> Items = TSoftObjectPtr<UDataTable>(
		FSoftObjectPath("/Game/LifeDev/Game/Inventory/Items"));

	// The data tables for the teachs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Inventory",
		meta=(RequiredAssetDataTags="RowStructure=/Script/Inventory.TeachRow"))
	TMap<ETeachTarget, TSoftObjectPtr<UDataTable>> TeachDTs = {{
		ETeachTarget::DESK, TSoftObjectPtr<UDataTable>(
		  FSoftObjectPath("/Game/LifeDev/Game/Inventory/TeachDesk.TeachDesk"))
		},{
		ETeachTarget::PAD, TSoftObjectPtr<UDataTable>(
		  FSoftObjectPath("/Game/LifeDev/Game/Inventory/TeachPad.TeachPad"))
		}};

	// the input mapping contexts used in game.
	// this is necessary so that the settings panel can rebind the keys
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Sys")
	TArray<TSoftObjectPtr<UInputMappingContext>> IMCs = {
		TSoftObjectPtr<UInputMappingContext>(FSoftObjectPath(TEXT("/Diags/Input/IMC_Dialogs.IMC_Dialogs"))),
		TSoftObjectPtr<UInputMappingContext>(FSoftObjectPath(TEXT("/Inventory/Input/IMC_Inventory.IMC_Inventory"))),
		TSoftObjectPtr<UInputMappingContext>(FSoftObjectPath(TEXT("/Diags/Input/IMC_Dialogs.IMC_Dialogs"))),
		TSoftObjectPtr<UInputMappingContext>(FSoftObjectPath(TEXT("/Game/LifeDev/Core/Settings/Input/Menu_IMC.Menu_IMC")))
	};

	// the chapter to start with.
	// during runtime use GetStartChap unless you're me and know what you're doing.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(ClampMax=4, ClampMin=0, UIMin=0, UIMax=4))
	int32 StartChap = -1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(RequiredAssetDataTags="RowStructure=/Script/LifeDev.LChapter"))
	TSoftObjectPtr<UDataTable> Chapters =
		TSoftObjectPtr<UDataTable>(FSoftObjectPath("/Game/LifeDev/Game/Dialogs/Chapters"));
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(RequiredAssetDataTags="RowStructure=/Script/Diags.DiagChar"))
	TSoftObjectPtr<UDataTable> Characters = TSoftObjectPtr<UDataTable>(
		FSoftObjectPath("/Game/LifeDev/Game/Dialogs/DiagChars"));
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(RequiredAssetDataTags="RowStructure=/Script/Diags.Diag"))
	TSoftObjectPtr<UDataTable> BaseDiags = TSoftObjectPtr<UDataTable>(
		FSoftObjectPath("/Game/LifeDev/Game/Dialogs/BaseDiags"));
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story",
		meta=(RequiredAssetDataTags="RowStructure=/Script/Diags.DiagGroup"))
	TSoftObjectPtr<UDataTable> BaseGroups = TSoftObjectPtr<UDataTable>(
		FSoftObjectPath("/Game/LifeDev/Game/Dialogs/BaseGroups"));
	// The base data layer that gets loaded when the game starts
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Story")
	TSoftObjectPtr<UDataLayerAsset> BaseDL = TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Base.Base"));

	// The default features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	TSet<EFeat> DefaultFeats = {
		EFeat::D_SHOW, EFeat::D_TEXT,
		EFeat::S_ENV, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_FOV, EFeat::V_BLUR, EFeat::V_MLIGHTS, EFeat::V_NANITE,
		EFeat::V_SPEED, EFeat::V_FLASHBACK, EFeat::V_STROBE, EFeat::V_DUST,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03, EFeat::C_04, EFeat::C_05
	};

	// The debug features
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	TSet<EFeat> DebugFeats = {
		EFeat::DBG_STEPS, EFeat::DBG_BASE,
		EFeat::D_SHOW,
		EFeat::S_ENV, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_FOV, EFeat::V_BLUR, EFeat::V_LUMEN, EFeat::V_MLIGHTS, EFeat::V_NANITE,
		EFeat::V_SPEED, EFeat::V_FLASHBACK, EFeat::V_STROBE, EFeat::V_DUST,
		EFeat::C_00, EFeat::C_01, EFeat::C_02, EFeat::C_03, EFeat::C_04, EFeat::C_05
	};

protected:
	// whether to use the feats listed in debugfeats or in defaultfeats
	// this is set to false on release builds.
	// only toggle this on the editor preferences, but during runtime call GetUseDebugFeats
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="Feats")
	bool UseDebugFeats = false;
};