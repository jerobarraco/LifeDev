#pragma once

// This is called misc utils because is the misc group
// later i plan on having more groups
#include "CoreMinimal.h"

class UInputMappingContext;
class UWorld;
class UWidget;

#include "JMiscUtils.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnJAsync);
DECLARE_DYNAMIC_DELEGATE(FOnJAsyncDone);

// Type of graph to use
UENUM(BlueprintType)
enum class EAsyncExec: uint8
	// Engine\Source\Runtime\Core\Public\Async\Async.h:27
	// the type used on AsyncTask is much more granular. but unfortunately it's not BlueprintType and is int32 so it cant be exposed
{
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
class JUTILS_API UJMiscUtils: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	// TODO implement to be able to use this
	// UFUNCTION(BlueprintCallable)
	// static UJMiscUtils* Get();

	UFUNCTION(BlueprintPure)
	static UWorld* GetEdWorld();

	UFUNCTION(BlueprintPure)
	static bool IsWithEditor();

	UFUNCTION(BlueprintCallable)
	static UWorld* JGetWorld(UWorld* World);

	UFUNCTION(BlueprintCallable)
	static void ToggleMapping(UInputMappingContext* Ctx, int32 Prio, bool Enable, UWorld* World);

	UFUNCTION(BlueprintCallable)
	static void ShowUI(bool Show, UWorld* World, UWidget* Focus = nullptr, bool SetPaused = false);

	// Calls a Task (a Delegate) on another thread, when finishes calls OnDone on the game thread (if bound)
	// What Not to Do:
	//	* Do not try to modify, create, or delete UObjects from other threads!
	//	* You can prepare all the data / do all the calculations, but only the game thread should be actually spawning / modifying / deleting UObjects / AActors.
	//	* Dont try to use TimerManager outside of the game thread :)
	//	* Don't try to draw debug lines/points etc, as it will likely crash, ie DrawDebugLine(etc...)
	UFUNCTION(BlueprintCallable)
	static void BPASync(const FOnJAsync& Task, const FOnJAsyncDone& Done, EAsyncExec Exec = EAsyncExec::ThreadPool);
};
