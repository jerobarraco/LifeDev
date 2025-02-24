// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once
#include "Engine/DataTable.h"

#include "DiagTypes.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogTextDialogs, Log, Log)

UENUM(BlueprintType)
enum class EDiagType : uint8 {
	NORMAL,
	SYSTEM,
	WHISPER,
	GROUP,
	MISTERY,
};

// Modifiers for a sequence
UENUM(BlueprintType)
enum class ESeqType : uint8 {
	// Normal sequence
	SEQUENCE,
	// Choose one dialog at random
	RANDOM,
	// Select one dialog depending on the condition, but clamp.
	PICK_CLAMP,
	// Select one dialog depending on the condition, but loop.
	PICK_LOOP,
	// Selects the first dialog that gets added successfully (implies passing its condition)
	PICK_MATCH,
	// Selects the one random dialog that gets added successfully (implies passing its condition)
	PICK_MATCH_RND,
};

// The base structure for dialogs.
// if the row ends with "*" it makes no difference (see what happens on sequences though). (this is a feature)
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDiag: public FTableRowBase {
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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Comment = "";
};

// The base structure for dialog group. For sequences, randoms, selections, etc.
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDiagGroup: public FTableRowBase {
	GENERATED_BODY()

public:
	// Behavior of the group
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESeqType Type = ESeqType::SEQUENCE;

	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> DiagRows;

	// Experimental. The sequence will be added if it's "true", or skipped otherwise. See Diags.CheckCondition for more info.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Condition = "";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Comment = "";
};

// chars
// structure to define a character
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDiagChar: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FLinearColor Color = FLinearColor::Gray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	TSoftObjectPtr<UTexture2D> Img = nullptr;
};
