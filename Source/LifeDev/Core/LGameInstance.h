// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Settings/LSysSettings.h"

#include "LGameInstance.generated.h"

class ULSave;

// LifeDev GameInstance
UCLASS()
class LIFEDEV_API ULGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	bool HasFeat(EFeat Feat);

	UFUNCTION(BlueprintCallable)
	void SetFeat(EFeat Feat, bool Enable = true);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TSet<EFeat> Feats;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ULSave* Save = nullptr;
};
