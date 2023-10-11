#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FLChapter.generated.h"
// can't be on the story plugin since it depends on data from other plugins. or will.

// a chapter
USTRUCT(BlueprintType)
struct LIFEDEV_API FLChapter: public FTableRowBase {
	GENERATED_BODY()

public:
	// dialog dialogs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RowType="Dialog"))
	TSoftObjectPtr<UDataTable> Dialogs;
	// dialog sequences
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RowType="DialogSequence"))
	TSoftObjectPtr<UDataTable> Sequences;
	
	// story steps belonging to this chapter
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> Steps;
};
