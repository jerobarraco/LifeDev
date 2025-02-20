// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once
#include "Engine/DataTable.h"

#include "DiagTypes.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogTextDialogs, Log, Log)

UENUM(BlueprintType)
enum class EDialogEmotion : uint8 {
	NEUTRAL,
	SAD,
	AFRAID,
	ANGRY,
	HAPPY,
	DISGUST,
};

UENUM(BlueprintType)
enum class EDialogType : uint8 {
	NORMAL,
	SYSTEM,
	WHISPER,
	GROUP,
	MISTERY,
};

// The base structure for dialogs.
// if the row ends with "*" it makes no difference (see what happens on sequences though). (this is a feature)
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDialog: public FTableRowBase {
	GENERATED_BODY()

public:
	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName CharRow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FText Text;

	// Experimental. The dialog will be added if it's "true", or skipped otherwise. See Diags.CheckCondition for more info.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Condition = "";
	
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// EDialogEmotion Emotion = EDialogEmotion::NEUTRAL;
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// EDialogType Type = EDialogType::NORMAL; // see clcharitems
	// WIP TODO
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// TMap<FName, FText> Options;
};

// The base structure for dialog sequences.
// If the key ends with "*" a _random_ dialog row will be picked.
// If it ends with a "!" it will choose the first dialog if the condition is true, the 2nd if false.
// If it ends with a "?" it will evaluate the condition and choose the corresponding dialog. Clamped to bounds (0, max).  
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDialogSequence: public FTableRowBase {
	GENERATED_BODY()

public:
	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> DiagRows;

	// Experimental. The sequence will be added if it's "true", or skipped otherwise. See Diags.CheckCondition for more info.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Condition = "";
};

// chars
// structure to define a character
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDialogChar: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FLinearColor Color = FLinearColor::Gray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	TSoftObjectPtr<UTexture2D> Img = nullptr;
};
