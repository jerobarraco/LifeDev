// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"

#include "JUtilsSys.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EJRHI: uint8 {
	VK_SM5 UMETA(DisplayName="Vulkan SM5"),
	VK_SM6 UMETA(DisplayName="Vulkan SM6"),
	VK_ES3 UMETA(DisplayName="Vulkan ES3.1"),
	MTL_SM5 UMETA(DisplayName="Metal SM5"),
	MTL_SM6 UMETA(DisplayName="Metal SM6"),
	MTL_ES3 UMETA(DisplayName="Metal ES3.1"),
	// vulkan on windows
	VK_WIN UMETA(DisplayName="Vulkan Windows"),
	DX11 UMETA(DisplayName="DirectX 11"),
	DX12 UMETA(DisplayName="Direct X12"),
	MAX UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EJRHI, EJRHI::MAX);


// TODO rename now it's outside of teach. Maybe EInputTarget
// todo create a input type or use one from ue. and use that instead.
// probably EHardwareDevicePrimaryType or EInputDevices
UENUM(Blueprintable, BlueprintType, meta=(Deprecated))
enum class ETeachTarget: uint8 {
	NONE,
	// desktop kb and mouse
	DESK, // rename to KBM
	// gamepad
	PAD,
	// Touchscreen (android?)
	TOUCH,
	VR,
	MAX UMETA(Hidden),
};

ENUM_RANGE_BY_COUNT(ETeachTarget, ETeachTarget::MAX);

UCLASS(Blueprintable)
class JUTILS_API UJUtilsSys: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
#pragma region info
	UFUNCTION(BlueprintCallable)
	static bool IsPIE();
	// also note FApp::GetBuildConfiguration()

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE bool IsEditor() {
		#if WITH_EDITOR
			return true;
		#else
			return false;
		#endif
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE bool IsDebug() {
		#if (UE_BUILD_TEST || UE_BUILD_SHIPPING)
			return false;
		#else
			return true;
		#endif
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE bool IsAndroid() {
		// return UGameplayStatics::GetPlatformName() == "Android";
		#if PLATFORM_ANDROID
			return true;
		#else
			return false;
		#endif
	}

	// returns the project version, as stored in the project settings
	// for the engine version there's a bp node from the engine already (engine version)
	UFUNCTION(BlueprintCallable)
	static void GetProjectVersion(FString& OVer);
	UFUNCTION(BlueprintCallable)
	static void GetUEBuildDate(FString& ODate);
	// this is also provided by kismet library
	UFUNCTION(BlueprintCallable)
	static void GetUEBuildVersion(FString& OVer);
	// returns the user's name, as presented by the platform.
	UFUNCTION(BlueprintCallable)
	static FString GetUserName();
	// sets the locale culture
	UFUNCTION(BlueprintCallable)
	static void SetCulture(const FString& Culture);
	// current locale culture
	UFUNCTION(BlueprintCallable)
	static const FString& GetCulture();
	// this one list all POSSIBLE not all supported :(
	UFUNCTION(BlueprintCallable)
	static void GetAllCultures(TArray<FString>& Names);
#pragma endregion

#pragma region camera
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void CameraFade(const UObject* const O, const bool In = false,
		const float Duration = .5f, const FLinearColor& Color = FLinearColor::Black);

	UFUNCTION(BlueprintCallable)
	static UGameViewportClient* GetAnyGameViewportClient();
#pragma endregion
#pragma region rhi
	// returns the number of (shaders?) precompilations remaining.
	// useful at the start of the game https://m.youtube.com/watch?v=HaVTYSnGvxA
	UFUNCTION(BlueprintCallable)
	static int64 NumPrecompilesRem();

	// returns a hardware info by name
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Name"))
	static void GetHWInfo(const FName& Name, FString& OutInfo);
	// gets the saved default rhi
	UFUNCTION(BlueprintCallable)
	static bool GetDefaultRHI(EJRHI &ORHI, FString& OutRHI);

	// gets the rhi in use
	UFUNCTION(BlueprintCallable)
	static void GetRHI(FString& OutRHI);

	// sets the default rhi. requires a restart.
	// make sure you choose the correct rhi for your platform, otherwise it will be skipped.
	// you also need to add to your DefaultEngine.ini the section to allow the file to be retained.
	// Or it will be deleted on start.
	// "[SectionsToSave] bCanSaveAllSections=true" https://forums.unrealengine.com/t/how-to-stop-local-custom-ini-files-being-deleted-after-play/1967813/2
	UFUNCTION(BlueprintCallable)
	static bool SetRHI(EJRHI RHI);
#pragma endregion
#pragma region input
	// returns the first local player controller. this is safe to use in network environments.
	// where the local player might not be the first controller.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static APlayerController* GetFirstLocalPlayerController(const UObject* const O);

	// WSettings notifies the UserSettings, otherwise they won't work (have to be enabled in the project settings)
	// This is necessary for being able to reset them
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void EInputToggleContext(const UObject* const O,
		const UInputMappingContext* const Ctx, const int32 Prio, const bool Enable,
		const bool WSetting = true);
	// returns the current enhanced input component
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputComponent* GetEInput(const UObject* const O);
	// returns the current enhanced input local player subsystem
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputLocalPlayerSubsystem* GetEInputSub(const UObject* const O);
	// returns the enhanced input user settings object
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputUserSettings* GetEInputSettings(const UObject* const O);
	// returns the current profile
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedPlayerMappableKeyProfile* GetEInputProfile(const UObject* const O);
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/BlueprintAPI/EnhancedInput/UserSettings/ResetMappingtoDefault
	// "Resets every player key mapping to this mapping back to its default value."
	// note, this and get profile requires that you enable "EnableUserSettings" on the project settings
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ResetEInputMapsAll(const UObject* const O);
	// https://forums.unrealengine.com/t/get-enhanced-input-local-player-subsystem-in-c/1732524/2
	// Resets the named mapping to its default value
	// N is the "Name" in the player mapping in the input action or context.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ResetEInputMap(const UObject* const O, const FName N);
	// Returns the Teach Target for a key
	UFUNCTION(BlueprintCallable)
	static FORCEINLINE ETeachTarget GetKeyTarget(const FKey& Key) {
		if (Key.IsGamepadKey()) return ETeachTarget::PAD;
		if (Key.IsTouch()) return ETeachTarget::TOUCH;
		return ETeachTarget::DESK;
	}
#pragma endregion // TODO maybe move to UtilsInput one day
};
