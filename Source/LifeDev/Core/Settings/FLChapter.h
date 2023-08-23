#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FLChapter.generated.h"

USTRUCT(BlueprintType)
struct LIFEDEV_API FLChapter: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Title;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RowType="Dialog"))
	TSoftObjectPtr<UDataTable> Dialogs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RowType="DialogSequence"))
	TSoftObjectPtr<UDataTable> Sequences;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> StorySeq;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<USoundBase> Music;
};
