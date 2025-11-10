// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "JUtilsInput.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;
class UInputMappingContext;

// EInputDevices is not exposed to bps!
UENUM(Blueprintable, BlueprintType)
enum class EInputType: uint8 {
	NONE,
	// desktop kb and mouse
	KBM, // rename to KBM
	// gamepad
	PAD,
	// Touchscreen (android?)
	TOUCH,
	VR,
	MAX UMETA(Hidden),
};
ENUM_RANGE_BY_COUNT(EInputType, EInputType::MAX);

UCLASS(Blueprintable)
class JUTILS_API UJUtilsInput : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	// returns the first local player controller. this is safe to use in network environments.
	// where the local player might not be the first controller.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static APlayerController* GetFirstLocalPlayerController(const UObject* const O);

	// WSettings notifies the UserSettings, otherwise they won't work (have to be enabled in the project settings)
	// This is necessary for being able to reset them
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ToggleContext(const UObject* const O,
		const UInputMappingContext* const Ctx, const int32 Prio, const bool Enable,
		const bool WSetting = true);
	// returns the current enhanced input component
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputComponent* GetInput(const UObject* const O);
	// returns the current enhanced input local player subsystem
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputLocalPlayerSubsystem* GetInputSub(const UObject* const O);
	// returns the enhanced input user settings object
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputUserSettings* GetInputSettings(const UObject* const O);
	// returns the current profile
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedPlayerMappableKeyProfile* GetInputProfile(const UObject* const O);
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/BlueprintAPI/EnhancedInput/UserSettings/ResetMappingtoDefault
	// "Resets every player key mapping to this mapping back to its default value."
	// note, this and get profile requires that you enable "EnableUserSettings" on the project settings
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ResetInputMapsAll(const UObject* const O);
	// https://forums.unrealengine.com/t/get-enhanced-input-local-player-subsystem-in-c/1732524/2
	// Resets the named mapping to its default value
	// N is the "Name" in the player mapping in the input action or context.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ResetInputMap(const UObject* const O, const FName N);
	// Returns the Teach Target for a key
	UFUNCTION(BlueprintCallable)
	static EInputType GetKeyType(const FKey& Key);
	// not force-inlining since that will force any plugin using this to also depend on inputcore unecessarily
};
