// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "JUtilsSys.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UJUtilsSys::ToggleMapping(UObject* O, UInputMappingContext* Ctx, int32 Prio, bool Enable) {
	APlayerController* const Controller = GetFirstLocalPlayerController(O);
	if (!IsValid(Controller)) return;
	
	UEnhancedInputLocalPlayerSubsystem* const Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			Controller->GetLocalPlayer());
	if (!IsValid(Subsystem)) return;

	if (Enable) Subsystem->AddMappingContext(Ctx, Prio);
	else Subsystem->RemoveMappingContext(Ctx);
}


APlayerController* UJUtilsSys::GetFirstLocalPlayerController(const UObject* const O) {
	const UWorld* const W = O?O->GetWorld():nullptr;
	if (!W) return nullptr;

	UGameInstance* const Instance = W->GetGameInstance();
	if (!Instance) return nullptr;

	return Instance->GetFirstLocalPlayerController(W);
}

UGameViewportClient* UJUtilsSys::GetAnyGameViewportClient() {
	if (GEngine && GEngine->GameViewport) return GEngine->GameViewport;
	
	// Then Game viewport is attached to another world context than the main Engine one. (ie: PIE Net mode set to As Client)
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if ((Context.WorldType == EWorldType::PIE) && Context.World() && Context.GameViewport)
		{
			return Context.GameViewport;
		}
	}

	return nullptr;
}


void UJUtilsSys::CameraFade(const UObject* const O, bool In, float Duration, const FLinearColor& Color) {
	const UWorld* const W = O?O->GetWorld():nullptr;
	if (!W) return;

	UGameInstance* const GI = W->GetGameInstance();
	if (!GI) {
		UE_LOG(LogTemp, Warning, TEXT("CameraFade: Invalid game instance. aborting"));
		return;
	}

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Fading screen."));	

	const APlayerController* const Controller = GI->GetPrimaryPlayerController();
	APlayerCameraManager* CamManager = Controller ? Controller->PlayerCameraManager : nullptr;
	if (!CamManager) {
		UE_LOG(LogTemp, Warning, TEXT("Can't get camera manager, not fading"));
		return;
	}

	const int32 From = In ? 1 : 0;
	const int32 To = In ? 0 : 1;
	CamManager->StartCameraFade(From, To, Duration, Color, true, true);
}



UEnhancedInputComponent* UJUtilsSys::GetEInput(const UObject* const O) {
	const APlayerController* const Controller = GetFirstLocalPlayerController(O);
	if (!Controller) return nullptr;

	UEnhancedInputComponent* const Input =
		Cast<UEnhancedInputComponent>(Controller->InputComponent);
	return Input;
}
