// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Story/Step.h"

#include "LStep.generated.h"

class UDialogs;

UCLASS(Blueprintable, BlueprintType)
class ALStep : public AStep {
	GENERATED_BODY()

public:
	virtual void Start_Implementation() override;
	
	virtual void Stop_Implementation() override;

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool InputEnabled = false;

	// dialog sequence to trigger on start
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName SeqId = NAME_None;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UDialogs* Dialogs = nullptr;
};
