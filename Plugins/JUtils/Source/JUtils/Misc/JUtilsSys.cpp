// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsSys.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Internationalization/Culture.h"

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

FString UJUtilsSys::GetProjectVersion() {
	// https://forums.unrealengine.com/t/how-to-get-the-project-version-in-a-blueprint/461882/2?u=nande
	FString Version;
	if (LIKELY(GConfig)) GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),Version, GGameIni);
	return MoveTemp(Version);
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
