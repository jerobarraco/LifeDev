// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "Engine/DataTable.h"

#include "DiagTypes.generated.h"

UENUM(BlueprintType)
enum class EDiagType : uint8 {
	NORMAL,
	WHISPER,
	GROUP,
	MISTERY,
	// TODO idea: you can use this one to trigger events. if you ignore them on the ui.
	SYSTEM,
	MAX UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EDiagType, EDiagType::MAX);

// Modifiers for a sequence
UENUM(BlueprintType)
enum class EGroupType : uint8 {
	// Normal sequential sequence.
	// The group needs to pass its condition.
	SEQUENCE,
	// Selects the *first* dialog that gets added successfully.
	// The group needs to pass its condition too.
	// Each dialog is tested *sequential* order. Only the first one passes its own dialog condition is shown.
	MATCH,
	// Selects *one* random dialog, the first to be added successfully.
	// The group needs to pass its own condition too.
	// Each dialog is tested in *random* order. Only the first that passes its condition is shown.
	// It will keep trying to add dialogs until one succeeds.
	RANDOM,
	// Select one dialog depending on the condition's result value, but clamp. (Note this always passes)
	PICK_CLAMP,
	// Select one dialog depending on the condition's result value, but loop. (Note this always passes)
	PICK_LOOP,
	MAX UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EGroupType, EGroupType::MAX);

// not entirely comfortable with this approach. if i change my mind, check lstep.cpp
// USTRUCT(Blueprintable, BlueprintType)
// struct DIAGS_API FDiagEffect {
	// GENERATED_BODY()

// public:
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// FName Name;
	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// bool Enable;
// };

// The base structure for dialogs.
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDiag: public FTableRowBase {
	GENERATED_BODY()

public:
	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName CharRow;
	// the text to show
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FText Text;

	// Emotion. was not a bad idea. i won't use it but other people might. maybe i can use tags. like yarn.

	// The dialog will be added if it's "true" (>0), or skipped otherwise. See Diags.CheckCondition for more info.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable)
	FString Condition = "";
	
	// a comment for your documentation. (or usage)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable)
	FString Comment = "";
	
	// Experimental: effects triggered by this dialog. is up to you to implement each effect.
	// DiagMan will call the function DoEffect whenever an effect is turned on or off.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> Effects;
};

// The base structure for dialog group. For sequences, randoms, selections, etc.
USTRUCT(Blueprintable, BlueprintType)
struct DIAGS_API FDiagGroup: public FTableRowBase {
	GENERATED_BODY()

public:
	// Behavior of the group
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGroupType Type = EGroupType::SEQUENCE;

	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> DiagRows;

	// Experimental. The sequence will be added if it's "true" (>0), or skipped otherwise. See Diags.CheckCondition for more info.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable)
	FString Condition = "";
	
	// a comment for your documentation. (or usage)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AssetRegistrySearchable)
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
