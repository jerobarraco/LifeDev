// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsSys.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/UserSettings/EnhancedInputUserSettings.h"
#include "HardwareInfo.h"
#include "ShaderPipelineCache.h"
#include "Internationalization/Culture.h"
#include "Kismet/GameplayStatics.h"

#include "JUtilsMisc.h"
#if WITH_EDITOR
#include "EditorScriptingHelpers.h"
#endif

bool UJUtilsSys::IsPIE() {
#if WITH_EDITOR
	return EditorScriptingHelpers::CheckIfInEditorAndPIE();
#else
	return false;
#endif
}

void UJUtilsSys::ToggleMapping(const UObject* const O,
	const UInputMappingContext* const Ctx, const int32 Prio, const bool Enable) {
	const APlayerController* const Controller = GetFirstLocalPlayerController(O);
	if (UNLIKELY(!IsValid(Controller))) return;

	UEnhancedInputLocalPlayerSubsystem* const Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			Controller->GetLocalPlayer());
	if (UNLIKELY(!IsValid(Subsystem))) return;

	if (Enable) Subsystem->AddMappingContext(Ctx, Prio);
	else Subsystem->RemoveMappingContext(Ctx);
}

APlayerController* UJUtilsSys::GetFirstLocalPlayerController(const UObject* const O) {
	const UWorld* const W = O ? O->GetWorld():nullptr;
	if (UNLIKELY(!W)) return nullptr;

	const UGameInstance* const Instance = W->GetGameInstance();
	if (UNLIKELY(!Instance)) return nullptr;

	return Instance->GetFirstLocalPlayerController(W);
}

UGameViewportClient* UJUtilsSys::GetAnyGameViewportClient() {
	if (UNLIKELY(!GEngine)) return nullptr;
	if (LIKELY(GEngine->GameViewport)) return GEngine->GameViewport;
	
	// Then Game viewport is attached to another world context than the main Engine one. (ie: PIE Net mode set to As Client)
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts) {
		if (Context.WorldType == EWorldType::PIE & bool(Context.GameViewport.Get())
			& bool(Context.World())) {
			return Context.GameViewport;
		}
	}

	return nullptr;
}

int64 UJUtilsSys::NumPrecompilesRem() {
	return FShaderPipelineCache::NumPrecompilesRemaining();
}

void UJUtilsSys::GetHWInfo(const FName& Name, FString& OutInfo) {
	OutInfo = FHardwareInfo::GetHardwareInfo(Name);
}

#if PLATFORM_LINUX
static const TCHAR* const _RHI_SECTION = TEXT("/Script/LinuxTargetPlatform.LinuxTargetSettings");
#elif PLATFORM_WINDOWS
static const TCHAR* const _RHI_SECTION = TEXT("/Script/WindowsTargetPlatform.WindowsTargetSettings");
#elif PLATFORM_MAC
static const TCHAR* const _RHI_SECTION = TEXT("/Script/MacTargetPlatform.MacTargetSettings");
#endif

static const TCHAR* const _RHI_KEY = TEXT("DefaultGraphicsRHI"); 

static const TCHAR* const _RHI_VAL[] = {
	TEXT("SF_VULKAN_SM5"),
	TEXT("SF_VULKAN_SM6"),
	TEXT("SF_VULKAN_ES31"),
	TEXT("SF_METAL_SM5"),
	TEXT("SF_METAL_SM6"),
	TEXT("SF_METAL_ES3_1"),
	TEXT("SF_VULKAN_SM6"),
	TEXT("DefaultGraphicsRHI_DX11"),
	TEXT("DefaultGraphicsRHI_DX12")
	TEXT("DefaultGraphicsRHI_VULKAN")
};

bool UJUtilsSys::GetDefaultRHI(EJRHI &ORHI, FString& OutRHI) {
	if (UNLIKELY(!GConfig)) return false;
	
	GConfig->GetString(_RHI_SECTION, _RHI_KEY, OutRHI, GEngineIni);
	ORHI = EJRHI::MAX;
	for (const EJRHI r : TEnumRange<EJRHI>()) {
		if (UJUtilsMisc::StringLooseEquals(OutRHI, FString(_RHI_VAL[uint8(r)]))) {
			ORHI = r;
			break;
		}
	}
	return true;
}

void UJUtilsSys::GetRHI(FString& OutRHI) {
	FApp::GetBuildConfiguration();
	// thanks youngjun eom https://forums.unrealengine.com/t/how-could-i-know-which-rhi-is-performing-on-the-currently-running-device/424599/2?u=nande
	// GetHWInfo(NAME_RHI, OutRHI); // returns "vulkan"
	// returns "Vulkan (SM5)". it's the easiest way i've found to get the current feature level.
	// i could use this one GetMaxSupportedFeatureLevel but i'd need to verify if it's supported
	// Engine\Source\Runtime\RHI\Private\Linux\LinuxDynamicRHI.cpp:99
	OutRHI = FApp::GetGraphicsRHI();
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/RHI/GetDynamicRHI?application_version=5.4
	// https://forums.unrealengine.com/t/knowing-which-rhi-has-been-used-in-a-project-from-the-code/455808/2
}

bool UJUtilsSys::SetRHI(EJRHI RHI) {
	// based a bit on https://github.com/Cesio137/UE4-GraphicsRHIManager/blob/main/Source/RHIManager/Private/RHIManagerBPLibrary.cpp
	if (UNLIKELY(!GConfig)) return false;

	// this also covers out of bounds
	bool WrongPlat = false;
#if PLATFORM_LINUX
	WrongPlat = (RHI != EJRHI::VK_SM6) & (RHI != EJRHI::VK_SM5) & (RHI != EJRHI::VK_ES3);
#elif PLATFORM_WINDOWS
	WrongPlat = (RHI != EJRHI::DX11) & (RHI != EJRHI::DX12) & (RHI != EJRHI::VK_WIN);
#elif PLATFORM_MAC
	WrongPlat = (RHI != EJRHI::MTL_SM5) & (RHI != EJRHI::MTL_SM6) & (RHI != EJRHI::MTL_ES3);
#endif

	if (WrongPlat) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Attempt to store an invalid rhi for the current platform."
			" Platform=%s, RHI=%s"), __func__,
			*UGameplayStatics::GetPlatformName(), *UEnum::GetValueAsString(RHI));
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("%hs Platform=%s, RHI=%s INI=%s"), __func__,
			*UGameplayStatics::GetPlatformName(), *UEnum::GetValueAsString(RHI), *GEngineIni);
	GConfig->SetString(_RHI_SECTION, _RHI_KEY, _RHI_VAL[uint8(RHI)], GEngineIni);
	GConfig->Flush(true, GEngineIni);
	return true;
}

void UJUtilsSys::CameraFade(const UObject* const O, const bool In, const float Duration,
	const FLinearColor& Color) {
	const UWorld* const W = LIKELY(O) ? O->GetWorld():nullptr;
	if (UNLIKELY(!W)) return;

	const UGameInstance* const GI = W->GetGameInstance();
	if (UNLIKELY(!GI)) {
		UE_LOG(LogTemp, Warning, TEXT("CameraFade: Invalid game instance. aborting"));
		return;
	}

	if(LIKELY(GEngine))
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Fading screen."));	

	const APlayerController* const Controller = GI->GetFirstLocalPlayerController(W);
	APlayerCameraManager* const CamManager = LIKELY(Controller) ? Controller->PlayerCameraManager : nullptr;
	if (UNLIKELY(!CamManager)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Can't get camera manager, not fading"), __func__);
		return;
	}

	const int32 From = In ? 1 : 0;
	const int32 To = In ? 0 : 1;
	CamManager->StartCameraFade(From, To, Duration, Color, true, true);
}

UEnhancedInputComponent* UJUtilsSys::GetEInput(const UObject* const O) {
	const APlayerController* const Controller = GetFirstLocalPlayerController(O);
	if (UNLIKELY(!Controller)) return nullptr;

	UEnhancedInputComponent* const Input =
		Cast<UEnhancedInputComponent>(Controller->InputComponent);
	return Input;
}

UEnhancedInputLocalPlayerSubsystem* UJUtilsSys::GetEInputSub(const UObject* const O) {
	const UWorld* const W = LIKELY(O) ? O->GetWorld() : nullptr;
	const ULocalPlayer* const Player = LIKELY(W) ? W->GetFirstLocalPlayerFromController() : nullptr;
	UEnhancedInputLocalPlayerSubsystem* const Subsystem =
		LIKELY(Player) ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player) : nullptr;
	return Subsystem;
}

UEnhancedPlayerMappableKeyProfile* UJUtilsSys::GetEInputProfile(const UObject* const O) {
	const UEnhancedInputLocalPlayerSubsystem* const Subsystem = GetEInputSub(O);
	const UEnhancedInputUserSettings* const Settings = LIKELY(Subsystem) ? Subsystem->GetUserSettings() : nullptr;
	UEnhancedPlayerMappableKeyProfile* const Profile = LIKELY(Settings) ? Settings->GetActiveKeyProfile() : nullptr;
	return Profile;
}

void UJUtilsSys::ResetEInputMapsAll(const UObject* const O) {
	UEnhancedPlayerMappableKeyProfile* const Profile = GetEInputProfile(O);
	if (LIKELY(Profile)) Profile->ResetToDefault();
	else UE_LOG(LogTemp, Warning, TEXT("%hs Could not get the current EnhancedPlayerMappableKeyProfile"), __func__);
}

void UJUtilsSys::ResetEInputMap(const UObject* const O, const FName N) {
	if (UNLIKELY(N.IsNone() | !O)) return; // !O is cheap so put it there too.

	UEnhancedPlayerMappableKeyProfile* const Profile = GetEInputProfile(O);
	if (LIKELY(Profile)) Profile->ResetMappingToDefault(N);
	else UE_LOG(LogTemp, Warning, TEXT("%hs Could not get the current EnhancedPlayerMappableKeyProfile"), __func__);
}

void UJUtilsSys::GetProjectVersion(FString& OVer) {
	// https://forums.unrealengine.com/t/how-to-get-the-project-version-in-a-blueprint/461882/2?u=nande
	if (LIKELY(GConfig)) GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"), OVer, GGameIni);
}

void UJUtilsSys::GetUEBuildDate(FString& ODate) {
	ODate = FApp::GetBuildDate();
}

void UJUtilsSys::GetUEBuildVersion(FString& OVer) {
	OVer = FApp::GetBuildVersion();
}

FString UJUtilsSys::GetUserName() {
	return FPlatformProcess::UserName(false);
}

void UJUtilsSys::SetCulture(const FString& Culture) {
	FInternationalization::Get().SetCurrentCulture(Culture);
}

const FString& UJUtilsSys::GetCulture() {
	const FCultureRef Culture = FInternationalization::Get().GetCurrentCulture();
	return Culture->GetName();
}

void UJUtilsSys::GetAllCultures(TArray<FString>& Names) {
	FInternationalization::Get().GetCultureNames(Names);
}
