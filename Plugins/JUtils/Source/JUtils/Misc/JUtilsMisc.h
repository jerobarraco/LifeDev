// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

// This is called misc utils because is the misc group
// later i plan on having more groups

#include <string>
#include "CoreMinimal.h"

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

#pragma region UI // TODO move to JUtilsUI
	// Shows/hides an ui. it will also show the mouse if needed.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ShowUI(const UObject* O, bool Show, UWidget* const Focus = nullptr);

	// this will set the ui scale factor. It will be saved on a config file.
	// you can change it back on the editor under "Project Settings > Engine > User Interface > Application Scale"
	// this, alongside the window scale, defined with a setting on the project,
	// will in turn adjust the DPI for the widgets and text dynamically.
	// you can obtain such DPI with UWidgetLayoutLibrary::GetViewportScale (it's exposed to bps)
	UFUNCTION(BlueprintCallable)
	static void SetUIScale(const float UIScale);
	// current ui scale factor
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetUIScale();
#pragma endregion // move to UtilsUI someday

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

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool ToggleDataLayer(const UObject* const O, const UDataLayerAsset* const DataLayer, const bool Enabled = true);

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
