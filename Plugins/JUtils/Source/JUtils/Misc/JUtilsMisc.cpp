// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsMisc.h"

#include <Async/Async.h>
#include "CoreGlobals.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "WorldPartition/DataLayer/DataLayerInstance.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogJUtilsMisc, Log, Log);

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

bool UJUtilsMisc::StringLooseEquals(const FString& A, const FString& B) {
	// Receives a copy since we will modify them. But using both inlines will be faster than calling Trim().Lower().
	return A.TrimStartAndEnd().Equals(B.TrimStartAndEnd(), ESearchCase::IgnoreCase);
}

bool UJUtilsMisc::ToggleDataLayer(const UObject* const O, const UDataLayerAsset* const DataLayer, const bool Enabled) {
	if (UNLIKELY(!O)) return false;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!World)) return false;

	if (UNLIKELY(!IsValid(DataLayer))) {
		UE_LOG(LogJUtilsMisc, Warning, TEXT("%hs Invalid data layer."), __func__);
		return false;
	}

	UDataLayerManager* const Manager = World->GetDataLayerManager();
	if (UNLIKELY(!IsValid(Manager))) {
		UE_LOG(LogJUtilsMisc, Warning, TEXT("%hs Could not get the data layer manager"), __func__);
		return false;
	}

	const EDataLayerRuntimeState State =
		(Enabled ? EDataLayerRuntimeState::Activated : EDataLayerRuntimeState::Unloaded);
	const bool Success = Manager->SetDataLayerRuntimeState(DataLayer, State, false);
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs Data layer toggle. Ok=%i, Enable=%i, Name='%s'"),
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

ULevelStreamingDynamic* UJUtilsMisc::LevelDynLoad(UObject* const O,
	const TSoftObjectPtr<UWorld> Level, const FTransform& Trans, const bool Visible) {

	bool LoadOk = false; 
	ULevelStreamingDynamic* const Stream = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		O, Level, Trans.GetLocation(), Trans.Rotator(), LoadOk);
	if (UNLIKELY(!LoadOk | !Stream)) {
		UE_LOG(LogJUtilsMisc, Warning, TEXT("%hs, Failed to load the streamed level. Stop."), __func__);
		return nullptr;
	}

	// done on flag only since setting SetShouldBeVisible(true) right after this function on the same frame seems to bring issues
	if (!Visible) {
		// should be visible is required. the side effect is that "begin play" of objects is not processed until the
		// visibility changes. but that is actually a good thing, otherwise is really hard to signal the sequence to start.
		// in other words, that quirk is a feature we will use.
		Stream->SetShouldBeVisible(false);
		Stream->bInitiallyVisible = false; // make sure it starts invisible
	}

	return Stream;
}

bool UJUtilsMisc::LevelDynUnload(const UObject* const O, ULevelStreamingDynamic* const Stream, const bool Block) {
	return LevelUnloadPtr(O, Stream, Block);
}

// level load and unload are not exposed to bp in ue. they are only available as latent actions. so we make our own.

ULevelStreaming* UJUtilsMisc::GetLevel(const UObject* const O, const FName Name) {
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs. Name=%s"), __func__, *Name.ToString());
	if (UNLIKELY(Name.IsNone())) return nullptr;
	if (UNLIKELY(!IsValid(O))) return nullptr;
	
	ULevelStreaming* const Level = UGameplayStatics::GetStreamingLevel(O, Name);
	UE_CLOG(UNLIKELY(!IsValid(Level)), LogJUtilsMisc, Warning, TEXT("%hs. Could not get the level. Name=%s"),
		__func__, *Name.ToString());

	return Level;
}

bool UJUtilsMisc::LevelLoad(const UObject* const O, const FName Name, const bool MakeVisible, const bool Block) {
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs. Name=%s Visible=%i Block=%i"),
		__func__, *Name.ToString(), MakeVisible, Block);
	if (UNLIKELY(Name.IsNone())) return false;
	if (UNLIKELY(!IsValid(O))) return false;

	FLatentActionInfo LAI;
	UGameplayStatics::LoadStreamLevel(O, Name, MakeVisible, Block, LAI);
	
	return true;
}

bool UJUtilsMisc::LevelUnload(const UObject* const O, const FName Name, const bool Block) {
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs. Name=%s Block=%i"),
		__func__, *Name.ToString(), Block);
	if (UNLIKELY(Name.IsNone())) return false;
	if (UNLIKELY(!IsValid(O))) return false;
	
	ULevelStreaming* const Level = GetLevel(O, Name);
	return LevelUnloadPtr(O, Level, Block);
}

bool UJUtilsMisc::LevelUnloadPtr(const UObject* const O, ULevelStreaming* const Level, const bool Block) {
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs. Name=%s Block=%i Package=%s"),
		__func__, *GetNameSafe(Level), Block, Level?*Level->GetWorldAssetPackageName():TEXT("???"));
	if (UNLIKELY(!IsValid(O))) return false;
	if (UNLIKELY(!IsValid(Level))) {
		UE_LOG(LogJUtilsMisc, Warning, TEXT("%hs Can't unload the level. It seems invalid. Stop."), __func__);
		return false;
	}
	
	Level->SetShouldBeVisible(true); // this is important.
	Level->SetShouldBeLoaded(false); // to be sure. not critical but sometimes helps.
	FLatentActionInfo LAI;
	LAI.UUID = Level->GetUniqueID(); // this is critical, or it won't unload multiple in a row.
	const TSoftObjectPtr<UWorld>& LevelWorld = Level->GetWorldAsset();
	// appears to be correct https://dev.epicgames.com/documentation/en-us/unreal-engine/loading-and-unloading-levels-using-cplusplus-in-unreal-engine
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(O, LevelWorld, LAI, Block);
	return true;
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
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs Try to load '%s'"), __func__, *Path);
	if (UNLIKELY(!FPaths::FileExists(Path))) return nullptr;

	FString S;
	if (UNLIKELY(!FFileHelper::LoadFileToString(S,*Path,FFileHelper::EHashOptions::None))) {
		UE_LOG(LogJUtilsMisc, Log, TEXT("%hs Can't read '%s'. Stop"), __func__, *Path);
		return nullptr;
	}

	UDataTable* const Table = NewObject<UDataTable>(Outer, UDataTable::StaticClass());
	Table->RowStruct = RowType; // important
	OProblems = Table->CreateTableFromJSONString(S);
	for (const FString& P: OProblems) {
		UE_LOG(LogJUtilsMisc, Warning, TEXT("%hs Problem on '%s' :'%s'"), __func__, *Path, *P);
	}

	// alternative return Problems.Num() <= 0 ? Table : nullptr;
	return Table;
}

void UJUtilsMisc::SetShaderBatchMode(const UObject* const O, const EShaderBatchMode Mode) {
	// https://www.tomlooman.com/psocaching-unreal-engine/
	// Use “Fast” for loading screens, “Background” for UI and interactive moments r.ShaderPipelineCache.SetBatchMode pause/fast/background/precompile
	if (UNLIKELY(!IsValid(O) | (Mode >= EShaderBatchMode::MAX))) return;

	static const TCHAR* const Base = TEXT("r.ShaderPipelineCache.SetBatchMode ");
	static const TCHAR* const Texts[] = { TEXT("Pause"), TEXT("Background"), TEXT("Fast"), TEXT("Precompile") };
	const FString Cmd = FString(Base) + Texts[static_cast<uint8>(Mode)];
	UKismetSystemLibrary::ExecuteConsoleCommand(O, Cmd);
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs Try to set shader batch mode to '%s'"), __func__, *Cmd);
}

void UJUtilsMisc::SetCVarChar(const TCHAR* Name, const int32 Val) {
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs %s=%i"), __func__, Name, Val);
	IConsoleVariable* const Variable = IConsoleManager::Get().FindConsoleVariable(Name);
	if (UNLIKELY(!Variable)) {
		UE_LOG(LogJUtilsMisc, Warning, TEXT("%hs Can't find var=%s. Stop"), __func__, Name);
		return;
	}

	Variable->Set(Val);
}

UObject* UJUtilsMisc::GetMutableDefault(const TSubclassOf<UObject>& Class, const bool CreateIfNeeded) {
	return Class.Get()->GetDefaultObject(CreateIfNeeded);
	// copied from 	GetMutableDefault<Class.Get()>();
}

UDataTable* UJUtilsMisc::LoadCSVTable(const FString& BasePath, const FString& Name, UScriptStruct* const RowType,
									TArray<FString>& OProblems, UObject* const Outer) {
	const FString& Path = FPaths::ConvertRelativePathToFull(FPaths::Combine(BasePath, Name+".csv"));
	UE_LOG(LogJUtilsMisc, Log, TEXT("%hs Try to load '%s'"), __func__, *Path);
	if (UNLIKELY(!FPaths::FileExists(Path))) return nullptr;

	FString S;
	if (UNLIKELY(!FFileHelper::LoadFileToString(S, *Path,FFileHelper::EHashOptions::None))) {
		UE_LOG(LogJUtilsMisc, Log, TEXT("%hs Can't read '%s'. Stop"), __func__, *Path);
		return nullptr;
	}

	UDataTable* const Table = NewObject<UDataTable>(Outer, UDataTable::StaticClass());
	Table->RowStruct = RowType; // important
	Table->CreateTableFromCSVString(S);
	
	OProblems = Table->CreateTableFromCSVString(S);
	for (const FString& P: OProblems) {
		UE_LOG(LogJUtilsMisc, Warning, TEXT("%hs Problem on '%s' :'%s'"), __func__, *Path, *P);
	}

	// TODO test
	return Table;

	// only for editor
	// UDataTableFunctionLibrary::FillDataTableFromCSVFile(Table, Path, Struct);
}
