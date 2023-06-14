// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"

#include "Step.generated.h"

class UCameraComponent;
class ACameraActor;

UCLASS(Blueprintable, BlueprintType)
class STORY_API AStep : public AActor {
	GENERATED_BODY()

public:
	AStep();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Start() ;
	virtual void Start_Implementation();

	// don't call this one directly. called by the system. Override to perform whatever the step needs to do at the end.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Stop();
	virtual void Stop_Implementation();

	// Call this one to stop the step from outside the system, or from the step itself.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Finish();
	virtual void Finish_Implementation();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AActor* CamTarget = nullptr;

	// will target pawn automatically, will override the camtarget
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UsePawnCam = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseCam = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float CamBlendTime = 2.0;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;
	void UpdateCamEnabled();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* Root = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=SetUp)
	UCameraComponent* Cam = nullptr;
};
