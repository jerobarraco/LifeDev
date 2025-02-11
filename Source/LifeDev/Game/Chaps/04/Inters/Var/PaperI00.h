// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "Components/TextRenderComponent.h"

#include "LifeDev/Game/Chaps/All/Inters/Papers/Paper00.h"

#include "PaperI00.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APaperI00: public APaper00 {
	GENERATED_BODY()

public:
	APaperI00();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void FadeUpdate(const float Progress, const float FadeV);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UTextRenderComponent> Text = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient)
	TObjectPtr<UMaterialInstanceDynamic> TextMID = nullptr;
};
