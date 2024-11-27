#pragma once

#include "CoreMinimal.h"

#include "JUtilsSys.generated.h"

class UInputMappingContext;

UCLASS(Blueprintable)
class JUTILS_API UJUtilsSys: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void CameraFade(const UObject* const O, bool In = false, float Duration = .5f, const FLinearColor& Color = FLinearColor::Black);

	UFUNCTION(BlueprintCallable)
	static UGameViewportClient* GetAnyGameViewportClient();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static void ToggleMapping(const UObject* const O,
		const UInputMappingContext* const Ctx, int32 Prio, bool Enable);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static APlayerController* GetFirstLocalPlayerController(const UObject* const O);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UEnhancedInputComponent* GetEInput(const UObject* const O);
};
