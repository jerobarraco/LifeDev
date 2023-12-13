// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"

#include "Step.generated.h"

class UCameraComponent;
class ACameraActor;
class UDataLayerAsset;
class USceneComponent;
class AActor;
class USoundBase;

UCLASS(Blueprintable, BlueprintType)
class STORY_API AStep : public AActor {
	GENERATED_BODY()

public:
	AStep();
	void DoTeleport();

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

	// >0 will set the seconds to wait since the start of this step. will trigger PostWait.
	// this affects the dialogs. See UseFadeTime
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float WaitTime = 0;

	// When set to true, it will fade in/out using the game fade time on start.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseFade = false;

	// teleports the character to where this cam is located. and also copies the rotation
	// beware this affects the camera blend if any (if blending from/to the character)
	// teleports before blending. teleports on start of step.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool TeleportChar = false;
	
	// Title to show. Only shown if you also set UseFade
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FText Title;

	// Music to play. Setting it to empty does not stop the music.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TSoftObjectPtr<USoundBase> Music;

	// Data layers to load.
	// make sure to mark the data-layer-asset as "runtime" or it won't work. (open it)
	// also in the data layers panel, set the initial runtime state.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<UDataLayerAsset*> DL_Load;
	// Data layers to unload
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<UDataLayerAsset*> DL_Unload;
	
	// set to true to use debug
	inline static bool UseDebug = false;

protected:
	// Will be triggered when the wait time ends. if it's set.
	// if you don't override or if you call the parent (this) it will finish the step.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PostWait();
	virtual void PostWait_Implementation();

	// gets called when UseDebug is set. happens on postwait
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Debug();
	virtual void Debug_Implementation() {};
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;
	void UpdateCamEnabled() const;
	void BlendCam() const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* Root = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=SetUp)
	UCameraComponent* Cam = nullptr;
};
