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

	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	void ResetFeats();

	UFUNCTION(BlueprintCallable)
	bool GetFeat(EFeat Feat) const;

	UFUNCTION(BlueprintCallable)
	void SetFeat(EFeat Feat, bool Enable = true);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TSet<EFeat> Feats;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ULSave* Save = nullptr;
};
