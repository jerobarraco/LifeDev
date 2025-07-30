// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "JUtilsUI.generated.h"

UCLASS(Blueprintable)
class JUTILS_API UJUtilsUI: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool GetStyleText(USlateWidgetStyleAsset* const Asset, FTextBlockStyle& Ret) {
		if (UNLIKELY(!Asset)) return false;
		Ret = *Asset->GetStyle<FTextBlockStyle>(); // copy
		return true;
	}
	
	UFUNCTION(BlueprintCallable)
	static bool GetStyleButton(USlateWidgetStyleAsset* const Asset, FButtonStyle& Ret) {
		if (UNLIKELY(!Asset)) return false;
		Ret = *Asset->GetStyle<FButtonStyle>(); // copy
		return true;
	}
};
