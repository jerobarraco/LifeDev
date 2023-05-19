#pragma once
#include "Engine/DataTable.h"

#include "DiagTypes.generated.h"

UENUM(BlueprintType)
enum class EDialogEmotion : uint8 {
	NEUTRAL,
	SAD,
	AFRAID,
	ANGRY,
	HAPPY,
	DISGUST,
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
	FText Text;
};

typedef TArray<FDialog> FDialogSeq;

// chars
// structure to define a character
USTRUCT(Blueprintable, BlueprintType)
struct DIALOGS_API FDialogChar: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	FLinearColor Color;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Dialogs")
	UTexture2D* Img = nullptr;
};
