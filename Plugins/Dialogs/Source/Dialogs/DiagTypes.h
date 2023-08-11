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
struct DIALOGS_API FDialog: public FTableRowBase {
	GENERATED_BODY()

public:
	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName CharRow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDialogEmotion Emotion = EDialogEmotion::NEUTRAL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDialogType Type = EDialogType::NORMAL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Text;
};

// The base structure for dialog sequences
USTRUCT(Blueprintable, BlueprintType)
struct DIALOGS_API FDialogSequence: public FTableRowBase {
	GENERATED_BODY()

public:
	// this is the row name in the datatable of type FChar
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> DiagRows;
};

// chars
// structure to define a character
USTRUCT(Blueprintable, BlueprintType)
struct DIALOGS_API FDialogChar: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FLinearColor Color = FLinearColor::Gray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	TSoftObjectPtr<UTexture2D> Img = nullptr;
};
