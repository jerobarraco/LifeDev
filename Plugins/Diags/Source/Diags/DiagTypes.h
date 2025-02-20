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

// The base structure for dialogs
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDialog: public FTableRowBase {
	GENERATED_BODY()

public:
	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName CharRow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FText Text;

	// EXPERIMENTAL
	// mathematical expression, that, when evaluated, if >0, it will add the dialog.
	// flags can be added like "{myflag}" then have operators like "/+-*"
	// a flag that is not set equals to 0
	// to be used you need to hook to the delegate in the diags subsystem to get the flags
	// e.g.:
	//	"{myflag}" triggers if the flag is set
	//	"{myflag-1}" flag is greater than 1
	//	"{-myflag}" flag is not set or negative
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
// If the key ends with "*" a dialog row will be picked.
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDialogSequence: public FTableRowBase {
	GENERATED_BODY()

public:
	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> DiagRows;

	// EXPERIMENTAL
	// mathematical expression, that, when evaluated, if >0, it will add the dialog.
	// flags can be added like "{myflag}" then have operators like "/+-*"
	// a flag that is not set equals to 0
	// to be used you need to hook to the delegate in the diags subsystem to get the flags
	// e.g.:
	//	"{myflag}" triggers if the flag is set
	//	"{myflag-1}" flag is greater than 1
	//	"{-myflag}" flag is not set or negative
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
