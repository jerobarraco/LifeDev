#pragma once

// This is called misc utils because is the misc group
// later i plan on having more groups
#include "CoreMinimal.h"

class UInputMappingContext;
class UWorld;
class UWidget;

#include "JMiscUtils.generated.h"


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
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWrapper, class UWrapper*, me);
// https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/13
// A simple wrapper for binding to delegates with extra parameters.
// To use: Inherit from this object, add parameters you need.
// then instantiate like this
// UCBChangeWrapper* const Wrapper = NewObject<UCBChangeWrapper>();
// Wrapper->CB = C;
// Wrapper->OnChange.AddUniqueDynamic(this, &UGroupBox::ResetSelected);
// C->OnCheckStateChanged.AddUniqueDynamic(Wrapper, &UCBChangeWrapper::Dispatch);
UCLASS(Blueprintable, BlueprintType)
class UWrapper : public UObject {
	GENERATED_BODY()

public:
	UFUNCTION()
	void Dispatch(bool IsChecked) {
		OnDispatch.Broadcast(this);
	};

	UPROPERTY(Transient)
	FOnWrapper OnDispatch;
};
