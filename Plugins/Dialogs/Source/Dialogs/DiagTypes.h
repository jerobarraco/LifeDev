#pragma once
#include "Engine/DataTable.h"

#include "DiagTypes.generated.h"

UENUM(BlueprintType)
enum class EDiagEmotion : uint8 {
	NEUTRAL,
	SAD,
	AFRAID,
	ANGRY,
	HAPPY,
	DISGUST,

	COUNT
};

USTRUCT(Blueprintable, BlueprintType)
struct DIALOGS_API FDialog: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 bg = -1; // background. TODO make an enum.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Char = -1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDiagEmotion Emotion = EDiagEmotion::NEUTRAL; // TODO make enum
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Text;
};

typedef TArray<FDialog> FDialogSeq;
