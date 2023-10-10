#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FLChapter.generated.h"

// TODO move to the story plugin
USTRUCT(BlueprintType)
struct LIFEDEV_API FLChapter: public FTableRowBase {
	GENERATED_BODY()

public:
	// TODO remove
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DeprecatedProperty))
	FText Title;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RowType="Dialog"))
	TSoftObjectPtr<UDataTable> Dialogs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RowType="DialogSequence"))
	TSoftObjectPtr<UDataTable> Sequences;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> StorySeq;

	// TODO remove
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DeprecatedProperty))
	TSoftObjectPtr<USoundBase> Music;
};
