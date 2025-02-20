// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

// This is called misc utils because is the misc group
// later i plan on having more groups

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

	/** Execute in Task Graph (for short running tasks). */
	TaskGraph,

	/** Execute in Task Graph on the main thread (for short running tasks). */
	TaskGraphMainThread,

	/** Execute in separate thread if supported (for long running tasks). */
	Thread,

	/** Execute in separate thread if supported or supported post fork (see FForkProcessHelper::CreateThreadIfForkSafe) (for long running tasks). */
	ThreadIfForkSafe,

	/** Execute in global queued thread pool. */
	ThreadPool,
};

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
	};
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE bool IsAndroid() {
		// return UGameplayStatics::GetPlatformName() == "Android";
		#if PLATFORM_ANDROID
				return true;
		#else
				return false;
		#endif
	}
	
	UFUNCTION(BlueprintCallable)
	static UWorld* JGetWorld(UWorld* World);

	// Shows/hides an ui. it will also show the mouse if needed.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ShowUI(const UObject* const O, const bool Show,
		UWidget* const Focus = nullptr, const bool SetPaused = false);

	// Calls a Task (a Delegate) on another thread, when finishes calls OnDone on the game thread (if bound)
	// What Not to Do:
	//	* Do not try to modify, create, or delete UObjects from other threads!
	//	* You can prepare all the data / do all the calculations, but only the game thread should be actually spawning / modifying / deleting UObjects / AActors.
	//	* Don't try to use TimerManager outside the game thread :)
	//	* Don't try to draw debug lines/points etc., as it will likely crash, ie DrawDebugLine(etc...)
	UFUNCTION(BlueprintCallable)
	static void BPAsync(const FOnJAsync& Task, const FOnJAsyncDone& Done, EAsyncExec Exec = EAsyncExec::ThreadPool);

	UFUNCTION(BlueprintCallable)
	static bool StringLooseEquals(const FString& A, const FString& B);
	UFUNCTION(BlueprintCallable)
	static float MathEvaluate(const FString& Expression);
	

	// can't be a blueprint callable since it's templatized
	template <typename T>
	static bool ReadTable(const UDataTable* DT, TArray<T>& OutRows);

	// returns the size of a static array in a static manner. (i.e int arr[10] = 10)
	template <typename T, std::size_t N>
	static inline constexpr std::size_t ArraySize( const T(&)[N] ) noexcept { return N; }

	// shuffles an array in place.
	template <typename T>
	static void ArrayShuffle(TArray<T>& Array);

	
};
