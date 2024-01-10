// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Diags/DiagTypes.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC2S001.generated.h"

class AGhosts;
class ALInteract;

// chap02 handles the boss appearing
UCLASS(Blueprintable, BlueprintType)
class ALStepC2S001 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC2S001();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FVector GhostPos = FVector(210,-42,-65);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	ALInteract* FakeChar = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;
	UFUNCTION()
	void DestroyActors();
	UFUNCTION()
	void ShowDlg(const FDialog& Diag);

	UPROPERTY(BlueprintReadOnly, Transient)
	AGhosts* Ghosts = nullptr;
};
