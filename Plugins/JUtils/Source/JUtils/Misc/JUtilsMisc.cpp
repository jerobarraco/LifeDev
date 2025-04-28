// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsMisc.h"

#include <Async/Async.h>
#include "CoreGlobals.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/UserInterfaceSettings.h"
#include "GameFramework/PlayerController.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "WorldPartition/DataLayer/DataLayerInstance.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"

// TODO fix packaging fails with this one
// https://www.reddit.com/r/unrealengine/comments/sbqb5k/comment/hu4c6ze/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
#if WITH_EDITOR
	#include "Editor.h"
	#include "EditorScriptingHelpers.h"
	#include "Kismet/DataTableFunctionLibrary.h"
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

void UJUtilsMisc::ShowUI(const UObject* const O, const bool Show,  UWidget* const Focus) {
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
}

void UJUtilsMisc::BPAsync(const FOnJAsync& Task, const FOnJAsyncDone& Done, EAsyncExec Exec) {
	// don't use [&Task, &Done] since that will break on calling BPAsync multiple times
	Async(static_cast<EAsyncExecution>(Exec), [Task, Done]{
		Task.ExecuteIfBound();
		// necessary or crash :C
		if (LIKELY(Done.IsBound()))
			AsyncTask(ENamedThreads::GameThread, [Done]{
				Done.ExecuteIfBound();
			});
	});
}

template <typename T>
bool UJUtilsMisc::ReadTable(const UDataTable* const DT, TArray<T>& OutRows) {
	OutRows.Empty();
	if (UNLIKELY(!IsValid(DT))) {
		UE_LOG(LogTemp, Error, TEXT("%hs Data Table is not valid or unassigned."), __func__);
		return false;
	}

	TArray<T*> RawRows;
	// Can't pass pointers to bps, and don't want null values either
	DT->GetAllRows<T>(TEXT(""), RawRows);
	OutRows.Reserve(RawRows.Num());
	for (const T* const Row: RawRows) {
		if (UNLIKELY(!Row)) continue;
		OutRows.Add(*Row);
	}

	return true;
}

// template <typename T>
// void UJUtilsMisc::ArrayShuffle(TArray<T>& Array) {
	// const int32 ArraySize = Array.Num();
	// for (int32 i = ArraySize - 1; LIKELY(i > 0); --i) {
		// const int32 RandomIndex = FMath::RandRange(0, i);
		// Array.Swap(i, RandomIndex);
	// }
// }

bool UJUtilsMisc::StringLooseEquals(const FString& A, const FString& B) {
	// Receives a copy since we will modify them. But using both inlines will be faster than calling Trim().Lower().
	return A.TrimStartAndEnd().Equals(B.TrimStartAndEnd(), ESearchCase::IgnoreCase);
}

bool UJUtilsMisc::ToggleDataLayer(const UObject* const O, const UDataLayerAsset* const DataLayer, const bool Enabled) {
	if (UNLIKELY(!O)) return false;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!World)) return false;

	if (UNLIKELY(!IsValid(DataLayer))) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Invalid data layer."), __func__);
		return false;
	}

	UDataLayerManager* const Manager = World->GetDataLayerManager();
	if (UNLIKELY(!IsValid(Manager))) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Could not get the data layer manager"), __func__);
		return false;
	}

	const EDataLayerRuntimeState State =
		(Enabled ? EDataLayerRuntimeState::Activated : EDataLayerRuntimeState::Unloaded);
	const bool Success = Manager->SetDataLayerRuntimeState(DataLayer, State, false);
	UE_LOG(LogTemp, Log, TEXT("%hs Data layer toggle. Ok=%i, Enable=%i, Name='%s'"),
		__func__, Success, Enabled, *DataLayer->GetName());
	return Success;
	// arigatou! https://kinnaji.com/2022/12/24/worldpartition-datalayer/
	
	/*  the subsystem  all is deprecated
	UDataLayerSubsystem* const Layers = World->GetSubsystem<UDataLayerSubsystem>();
	UDataLayerInstance* Instance = Layers->GetDataLayerInstanceFromAsset(DLA);
	Layers->SetDataLayerRuntimeState(Instance, State, true);
	if (!IsValid(Layers)) {
		UE_LOG(LogStory, Warning, TEXT("Could not get the data layer subsystem"), On, *DLA->GetName());
		return;
	}
	*/
}

bool UJUtilsMisc::ObjectLabel(const UObject* const Object, FString& OLabel) {
	if (UNLIKELY(!IsValid(Object))) return false;

	const UClass* const Class = Object->GetClass();
	Class->GetName(OLabel);
	// if (Cast<UBlueprint>(Class->ClassGeneratedBy)) // classGeneratedBy doesn't exist on runtime
		// OLabel.RemoveFromEnd(TEXT("_C"), ESearchCase::CaseSensitive);

	// I've added an extra field to the interacts, BUT jutils can't/shouldn't depend on interacts :(
	return true;
}

UDataTable* UJUtilsMisc::LoadJSONTable(const FString& BasePath, const FString& Name, UScriptStruct* const RowType,
	TArray<FString>& OProblems, UObject* const Outer) {
	const FString& Path = FPaths::ConvertRelativePathToFull(FPaths::Combine(BasePath, Name+".json"));
	UE_LOG(LogTemp, Log, TEXT("%hs Try to load '%s'"), __func__, *Path);
	if (UNLIKELY(!FPaths::FileExists(Path))) return nullptr;

	FString S;
	if (UNLIKELY(!FFileHelper::LoadFileToString(S,*Path,FFileHelper::EHashOptions::None))) {
		UE_LOG(LogTemp, Log, TEXT("%hs Can't read '%s'. Stop"), __func__, *Path);
		return nullptr;
	}

	UDataTable* const Table = NewObject<UDataTable>(Outer, UDataTable::StaticClass());
	Table->RowStruct = RowType; // important
	OProblems = Table->CreateTableFromJSONString(S);
	for (const FString& P: OProblems) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Problem on '%s' :'%s'"), __func__, *Path, *P);
	}

	// return Problems.Num() <= 0 ? Table : nullptr;
	return Table;
}

UDataTable* UJUtilsMisc::LoadCSVTable(const FString& BasePath, const FString& Name, UScriptStruct* const RowType,
	TArray<FString>& OProblems, UObject* const Outer) {
	const FString& Path = FPaths::ConvertRelativePathToFull(FPaths::Combine(BasePath, Name+".csv"));
	UE_LOG(LogTemp, Log, TEXT("%hs Try to load '%s'"), __func__, *Path);
	if (UNLIKELY(!FPaths::FileExists(Path))) return nullptr;

	FString S;
	if (UNLIKELY(!FFileHelper::LoadFileToString(S,*Path,FFileHelper::EHashOptions::None))) {
		UE_LOG(LogTemp, Log, TEXT("%hs Can't read '%s'. Stop"), __func__, *Path);
		return nullptr;
	}

	UDataTable* const Table = NewObject<UDataTable>(Outer, UDataTable::StaticClass());
	Table->RowStruct = RowType; // important
	Table->CreateTableFromCSVString(S);
	
	OProblems = Table->CreateTableFromCSVString(S);
	for (const FString& P: OProblems) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Problem on '%s' :'%s'"), __func__, *Path, *P);
	}

	// TODO test
	return Table;

	// only for editor
	// UDataTableFunctionLibrary::FillDataTableFromCSVFile(Table, Path, Struct);
}

void UJUtilsMisc::SetUIScale(const float UIScale) {
	// inspired on https://benui.ca/unreal/ui-scale/
	UUserInterfaceSettings* const UISettings =
		GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());

	if (UNLIKELY(!UISettings)) return;
	UISettings->ApplicationScale = UIScale;
}

float UJUtilsMisc::GetUIScale() {
	const UUserInterfaceSettings* const UISettings =
		GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());

	if (UNLIKELY(!UISettings)) return 1;
	return UISettings->ApplicationScale;
}
