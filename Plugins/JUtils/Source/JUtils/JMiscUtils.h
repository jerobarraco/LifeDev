#pragma once

#include "JMiscUtils.generated.h"
UCLASS(Blueprintable)
class JUTILS_API UJMiscUtils: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	// TODO implement to be able to use this
	// UFUNCTION(BlueprintCallable)
	// static UJMiscUtils* Get();
	
	UFUNCTION(BlueprintCallable)
	UWorld* JGetWorld(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void ToggleMapping(UInputMappingContext* Ctx, int32 Prio, bool Enable, UWorld* World);
};
