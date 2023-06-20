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

	// the camera to switch to automatically. Set to null to not switch to.
	// By default is the own camera. or you can set UsePanwCam to use the pawn's camera 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AActor* CamTarget = nullptr;

	// will target pawn automatically, will override the camtarget
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UsePawnCam = false;

	// if set it will finish after the wait time. if wait time is 0 it will finish immediately.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool FinishPostWait = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float CamBlendTime = 2.0;

	// >0 will set the seconds to wait since the start of this step. will trigger TimerEnded. see note.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float WaitTime = 0;

protected:
	// Will be triggered when the wait time ends. if it's set.
	// if you don't override or if you call the parent (this) it will finish the step.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PostWait();
	virtual void PostWait_Implementation();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;
	void UpdateCamEnabled();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* Root = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=SetUp)
	UCameraComponent* Cam = nullptr;
};
