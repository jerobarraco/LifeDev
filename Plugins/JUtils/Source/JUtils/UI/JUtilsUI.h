// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Style/JSlateStyles.h"

#include "JUtilsUI.generated.h"

class UDelegateWrapper;
// note, this is transitional until epic decides to allow creating a combo item style using a slate widget style asset
UCLASS(BlueprintType)
class UJTableRowStyle : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTableRowStyle Style;
};

UCLASS(Blueprintable)
class JUTILS_API UJUtilsUI: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
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
