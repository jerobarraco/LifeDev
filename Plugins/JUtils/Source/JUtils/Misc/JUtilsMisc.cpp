// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "JUtilsMisc.h"
#include "CoreGlobals.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// TODO fix packaging fails with this one
// https://www.reddit.com/r/unrealengine/comments/sbqb5k/comment/hu4c6ze/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
#if WITH_EDITOR
	#include "Editor.h"
	#include "EditorScriptingHelpers.h"
#else
	#include "Engine/Engine.h"
#endif

// UJMiscUtils* UJMiscUtils::Get() {
	// return nullptr;
// }

UWorld* UJUtilsMisc::GetEditorWorld() {
#if WITH_EDITOR
	if (UNLIKELY(!GEditor)) return nullptr;
	return GEditor->GetEditorWorldContext(false).World(); 
#else
	return nullptr;
#endif
}

UWorld* UJUtilsMisc::GetPIEWorld(const int32 Num) {
#if WITH_EDITOR
	if (UNLIKELY(!GEditor)) return nullptr;
	const FWorldContext* const Context = GEditor->GetPIEWorldContext(Num);
	return Context ? Context->World() : nullptr;
	// if (EditorScriptingHelpers::CheckIfInEditorAndPIE()) { // TODO
		// FWorldContext* const PieContext = GEditor->GetPIEWorldContext(0);
		// return PieContext ? PieContext->World() : nullptr;
	// }
#else
	return nullptr;
#endif
}

bool UJUtilsMisc::IsPIE() {
#if WITH_EDITOR
	return EditorScriptingHelpers::CheckIfInEditorAndPIE();
#else
	return false;
#endif
}

UWorld* UJUtilsMisc::JGetWorld(UWorld* World) {
	if (IsValid(World)) return World;
/*
	*const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
	if (Context.World()->IsGameWorld() && Context.World()->GetNetMode() != ENetMode::NM_DedicatedServer)
	{
	DebugDrawingWorld = Context.World();
	break;
	}
	}*/
	// todo try above
	// World = GetWorld(); // this is static cant call this

	// TODO this is not working as expected.
	World = GetEditorWorld();
	if (UNLIKELY(!World)) World = GEngine->GetWorld();

	return World;
}

void UJUtilsMisc::ShowUI(const UObject* const O, const bool Show,  UWidget* const Focus, const bool SetPaused) {
	if (UNLIKELY(!IsValid(O))) return;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	APlayerController* const Controller = World->GetFirstPlayerController();
	if (UNLIKELY(!IsValid(Controller))) return;

	if (Show) {
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
			Controller, Focus, EMouseLockMode::DoNotLock, true);
	} else UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);

	Controller->SetShowMouseCursor(Show);

	if (SetPaused) UGameplayStatics::SetGamePaused(World, Show);
}

void UJUtilsMisc::BPAsync(const FOnJAsync& Task, const FOnJAsyncDone& Done, EAsyncExec Exec) {
	// don't use [&Task, &Done] since that will break on calling BPAsync multiple times
	Async(static_cast<EAsyncExecution>(Exec), [Task, Done]{
		Task.ExecuteIfBound();
		// necessary or crash :C
		if (Done.IsBound()) AsyncTask(ENamedThreads::GameThread, [Done]{
			Done.ExecuteIfBound();
		});
	});
}

template <typename T>
bool UJUtilsMisc::ReadTable(const UDataTable* DT, TArray<T>& OutRows) {
	OutRows.Empty();
	if (!IsValid(DT)) {
		UE_LOG(LogTemp, Error, TEXT("Data Table is not valid or unassigned."));
		return false;
	}
	
	TArray<T*> RawRows;
	// Can't pass pointers to bps, and don't want null values either
	DT->GetAllRows<T>(TEXT(""), RawRows);
	for (const T* Row: RawRows) {
		if (!Row) continue;
		OutRows.Add(*Row);
	}
	return true;
}

template <typename T>
void UJUtilsMisc::ArrayShuffle(TArray<T>& Array) {
	const int32 ArraySize = Array.Num();
	for (int32 i = ArraySize - 1; i > 0; --i) {
		const int32 RandomIndex = FMath::RandRange(0, i);
		Array.Swap(i, RandomIndex);
	}
}

bool UJUtilsMisc::StringLooseEquals(const FString& A, const FString& B) {
	// Receives a copy since we will modify them. But using both inlines will be faster than calling Trim().Lower().
	return A.TrimStartAndEnd().Equals(B.TrimStartAndEnd(), ESearchCase::IgnoreCase);
}
