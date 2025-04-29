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
	// dialog Diags
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RowType="Dialog")) // RowType does not work
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RequiredAssetDataTags="RowStructure=/Script/Diags.Diag"))
	TSoftObjectPtr<UDataTable> Dialogs;
	// dialog groups
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(RequiredAssetDataTags="RowStructure=/Script/Diags.DiagGroup"))
	TSoftObjectPtr<UDataTable> Groups;
	
	// story steps belonging to this chapter
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> Steps;
};
