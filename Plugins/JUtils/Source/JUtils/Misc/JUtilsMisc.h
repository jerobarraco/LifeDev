// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

// This is called misc utils because is the misc group
// later i plan on having more groups

#include <string>
#include "CoreMinimal.h"

class ULevelStreamingDynamic;
class UInputMappingContext;
class UWorld;
class UWidget;

#include "JUtilsMisc.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnJAsync);
DECLARE_DYNAMIC_DELEGATE(FOnJAsyncDone);

// Type of graph to use
UENUM(BlueprintType)
enum class EAsyncExec: uint8 {
	// Engine\Source\Runtime\Core\Public\Async\Async.h:27
	// the type used on AsyncTask is much more granular. but unfortunately it's not BlueprintType and is int32 so it cant be exposed
	// Execute in Task Graph (for short running tasks).
	TaskGraph,
	// Execute in Task Graph on the main thread (for short running tasks).
	TaskGraphMainThread,
	//Execute in separate thread if supported (for long-running tasks).
	Thread,
	// Execute in separate thread if supported or supported post fork (see FForkProcessHelper::CreateThreadIfForkSafe) (for long running tasks).
	ThreadIfForkSafe,
	// Execute in global queued thread pool.
	ThreadPool,

	MAX UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAsyncExec, EAsyncExec::MAX);

// The shader pipeline batch mode
// Sets the compilation batch mode, which should be one of:\n\tPause: .\n\tBackground: Low priority precompilation.\n\tFast: High priority precompilation.
// Engine\Source\Runtime\RenderCore\Private\ShaderPipelineCache.cpp:259
UENUM(BlueprintType)
enum class EShaderBatchMode: uint8 {
	// Suspend precompilation
	PAUSE,
	// Low priority. During gameplay. slower but will try not to stutter.
	// The maximum batch size is defined by r.ShaderPipelineCache.BackgroundBatchSize
	BACKGROUND,
	// High priority. For loading screens, will mess with the times in render thread.
	// The maximum batch size is defined by r.ShaderPipelineCache.BatchSize
	FAST,
	// The maximum batch size is defined by r.ShaderPipelineCache.PrecompileBatchSize
	PRECOMPILE,
	MAX UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EShaderBatchMode, EShaderBatchMode::MAX);

UCLASS(Blueprintable)
class JUTILS_API UJUtilsMisc: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	// TODO implement to be able to use this
	// UFUNCTION(BlueprintCallable)
	// static UJMiscUtils* Get();

	UFUNCTION(BlueprintCallable)
	static UWorld* GetEditorWorld();
	
	UFUNCTION(BlueprintCallable)
	static UWorld* GetPIEWorld(const int32 Num = 0);

	UFUNCTION(BlueprintCallable)
	static UWorld* JGetWorld(UWorld* const World);

	// Calls a Task (a Delegate) on another thread, when finishes calls OnDone on the game thread (if bound)
	// What Not to Do:
	//	* Do not try to modify, create, or delete UObjects from other threads!
	//	* You can prepare all the data / do all the calculations, but only the game thread should be actually spawning / modifying / deleting UObjects / AActors.
	//	* Don't try to use TimerManager outside the game thread :)
	//	* Don't try to draw debug lines/points etc., as it will likely crash, ie DrawDebugLine(etc...)
	UFUNCTION(BlueprintCallable)
	static void BPAsync(const FOnJAsync& Task, const FOnJAsyncDone& Done, EAsyncExec Exec = EAsyncExec::ThreadPool);

	// returns true when two strings are similar, ignoring case and whitespace.
	UFUNCTION(BlueprintCallable)
	static bool StringLooseEquals(const FString& A, const FString& B);
#pragma region levels
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool ToggleDataLayer(const UObject* const O, const UDataLayerAsset* const DataLayer, const bool Enabled = true);
	
	// loads an *instance* of a level. (dynamic)
	// this is a level that is not in the "Levels" panel.
	// Allows to load the same level multiple times. and it's more memory efficient.
	// O: world context object
	// Level: the level to load
	// Trans: the transform used for the loaded level
	// Visible: whether to make it visible on load
	// Returns the LevelStreamingDynamic object. Keep this object in order to unload.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ULevelStreamingDynamic* LevelDynLoad(
		UObject* const O, const TSoftObjectPtr<UWorld> Level, const FTransform& Trans = FTransform(),
		const bool Visible=false);

	// unloads a Dynamic level.
	// block: whether to block on unload
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool LevelDynUnload(const UObject* const O, ULevelStreamingDynamic* const Stream,
		const bool Block = false);

	// attempts to return a level by its name. mostly for the ones in the "levels" panel.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ULevelStreaming* GetLevel(const UObject* const O, const FName Name);
	// loads a level specified in the "Levels" panel.
	// can only be loaded once, and can be loaded blocking.
	// can't use transforms. and it's less memory efficient.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool LevelLoad(const UObject* const O, const FName Name,
		const bool MakeVisible = true, const bool Block = false);
	// Unloads a level specified in the "Levels" panel.
	// Potentially loaded by LevelLoad or set visible from a sequence visibility track.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool LevelUnload(const UObject* const O, const FName Name, const bool Block = false);
	// Unloads a level specified in the "Levels" panel.
	// Potentially loaded by LevelLoad or set visible from a sequence visibility track.
	// Takes a object (pointer) version.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool LevelUnloadPtr(const UObject* const O, ULevelStreaming* const Level, const bool Block = false);
#pragma endregion
#pragma region objects
	// returns the Classname. without the _C for bps.
	// returns the "default" actor label for an actor. Not the REAL actor label (that does not exist on runtime builds)
	// copied from AActor::GetDefaultActorLabel
	UFUNCTION(BlueprintCallable)
	static bool ObjectLabel(const UObject* const Object, FString& OLabel);

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	static FORCEINLINE bool NeedsInit(const UObject* const Object) {
		return bool(Object) & Object->HasAnyFlags(RF_NeedInitialization);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	static FORCEINLINE bool IsInConstructor() {
		// https://forums.unrealengine.com/t/how-to-tell-if-a-function-is-called-by-a-constructor/413102/6?u=nande
		const FUObjectThreadContext& ThreadContext = FUObjectThreadContext::Get();
		return ThreadContext.IsInConstructor > 0;
	}

	// Get the default object from the class. Optionally creating it if it's not there yet.
	// TODO test
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	static UObject* GetMutableDefault(const TSubclassOf<UObject>& Class, const bool CreateIfNeeded=true);

#pragma endregion
#pragma region tables
	UFUNCTION(BlueprintCallable)
	static UDataTable* LoadCSVTable(const FString& BasePath, const FString& Name,
		UScriptStruct* const RowType, TArray<FString>& OProblems, UObject* const Outer = nullptr);

	UFUNCTION(BlueprintCallable)
	static UDataTable* LoadJSONTable(const FString& BasePath, const FString& Name,
		UScriptStruct* const RowType, TArray<FString>& OProblems, UObject* const Outer = nullptr);

	// can't be a blueprint callable since it's templatized
	template <typename T>
	static bool ReadTable(const UDataTable* const DT, TArray<T>& OutRows) {
		if (UNLIKELY(!IsValid(DT))) {
			UE_LOG(LogTemp, Error, TEXT("%hs Data Table is not valid or unassigned."), __func__);
			OutRows.Empty(); // optimization to not do empty before time.
			return false;
		}

		TArray<T*> RawRows;
		// Can't pass pointers to bps, and don't want null values either
		DT->GetAllRows<T>(TEXT(""), RawRows);
		OutRows.Empty(RawRows.Num());
		for (const T* const Row: RawRows) {
			if (UNLIKELY(!Row)) continue;
			OutRows.Add(*Row);
		}

		return true;
	}
#pragma endregion

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void SetShaderBatchMode(const UObject* const O, const EShaderBatchMode Mode);
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O", AutoCreateRefTerm="Name"))
	static void SetCVar(const FString& Name, const int32 Val) {SetCVarChar(*Name, Val);};
	static void SetCVarChar(const TCHAR* Name, const int32 Val);
	
	// shuffles an array in place.
	// has to be inlined or the compiler won't find the definition
	template <typename T>
	static inline void ArrayShuffle(TArray<T>& Array) {
		const int32 ArraySize = Array.Num();
		for (int32 i = ArraySize - 1; LIKELY(i >= 0); --i) {
			const int32 RandomIndex = FMath::RandRange(0, i);
			if (LIKELY(i!=RandomIndex))
				Array.Swap(i, RandomIndex);
		}
	};

	// returns the size of a static array in a static manner. (i.e. int arr[10] = 10)
	template <typename T, std::size_t N>
	static inline constexpr std::size_t ArraySize( const T(&)[N] ) noexcept { return N; }

	static inline constexpr std::size_t TextLen(const TCHAR* const T) noexcept {
		return std::char_traits<TCHAR>::length(T);
	}

	// template functions need to be inlined >_> https://stackoverflow.com/a/456716
};
