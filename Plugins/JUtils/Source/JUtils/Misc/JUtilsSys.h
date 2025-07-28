// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "JUtilsSys.generated.h"

class UInputMappingContext;
UENUM(BlueprintType)
enum class EJRHI: uint8 {
	DX12,
	DX11,
	SM5,
	SM6,
	METAL,
	VULKAN_ES3,
	METAL_ES3,
	MAX UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EJRHI, EJRHI::MAX);

UCLASS(Blueprintable)
class JUTILS_API UJUtilsSys: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool IsPIE();

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
	static FString GetProjectVersion();

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
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void CameraFade(const UObject* const O, const bool In = false,
		const float Duration = .5f, const FLinearColor& Color = FLinearColor::Black);

	UFUNCTION(BlueprintCallable)
	static UGameViewportClient* GetAnyGameViewportClient();
	// returns the number of (shaders?) precompilations remaining.
	// useful at the start of the game https://m.youtube.com/watch?v=HaVTYSnGvxA
	UFUNCTION(BlueprintCallable)
	static int64 NumPrecompilesRem();

	UFUNCTION(BlueprintCallable)
	static bool GetRHI(FString& OutRHI);

	// sets the default rhi. requires a restart.
	UFUNCTION(BlueprintCallable)
	static bool SetRHI(EJRHI RHI);

#pragma region input
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ToggleMapping(const UObject* const O,
		const UInputMappingContext* const Ctx, const int32 Prio, const bool Enable);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static APlayerController* GetFirstLocalPlayerController(const UObject* const O);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputComponent* GetEInput(const UObject* const O);
#pragma endregion // TODO maybe move to UtilsInput one day
};
