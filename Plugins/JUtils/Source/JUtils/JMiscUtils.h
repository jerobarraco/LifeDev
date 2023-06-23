#pragma once

// TODO rename to JUtils

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
};
