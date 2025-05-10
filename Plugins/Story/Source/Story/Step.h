// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "Step.generated.h"

class UCameraComponent;
class ACameraActor;
class UDataLayerAsset;
class USceneComponent;
class AActor;
class USoundBase;

// base class for story steps
UCLASS(Blueprintable, BlueprintType, Config=Story, DefaultConfig)
class STORY_API AStep : public AActor {
	GENERATED_BODY()

public:
	AStep();

	// Call this one to stop the step from outside the system, or from the step itself.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(ForceAsFunction))
	void Finish();

	// teleports the player to where this step is located. including view point.
	UFUNCTION(BlueprintCallable)
	void DoTeleport();

	// name of the step. critically necessary. for the step system to start it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	FName Name = NAME_None;

	// Actor holding the camera to blend to. Or null to not use it.
	// By default, it's this own step. or you can set UsePawnCam to use the pawn's camera.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam")
	TSoftObjectPtr<AActor> CamTarget = nullptr;

	// will target pawn automatically, will override the camTarget
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam")
	bool UsePawnCam = false;
	// time used to animate the change of cameras. Only where CamTarget is valid
	// blend and fade could have some issues when used together.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam", Config)
	float CamBlendTime = 2;
	// shake the camera on start. stops on stop.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam")
	bool UseCamShake = false;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp|Cam")
	TSubclassOf<UCameraShakeBase> CamShakeClass = nullptr;

	// if set it will finish after the wait time. if wait time is 0 it will finish immediately.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transition")
	bool FinishPostWait = false;

	// >0 will set the seconds to wait since the TryStart of this step. will trigger Start (override it).
	// This ONLY happens on Start. This affects the dialogs (the main usage). See UseFadeTime.
	// if a cam blend needs to happen, the wait will be clamped to _at least_ CamBlendTime.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transition")
	float WaitTime = 0;

	// When set to true, it will fade in/out using the game fade time on start.
	// blend and fade could have some issues when used together.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transition")
	bool UseFade = false;

	// teleports the character to where this cam is located. and also copies the rotation
	// beware this affects the camera blend if any (if blending from/to the character)
	// teleports before blending. teleports on start of step. Rotation is broken.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	bool TeleportChar = false;

	// Title to show. Only shown if you also set UseFade
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transition")
	FText Title = FText::GetEmpty();

	// Music to play. Setting it to empty does not stop the music.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	TSoftObjectPtr<USoundBase> Music = nullptr;
	// TODO fix not packaging*

	// Data layers to load.
	// make sure to mark the data-layer-asset as "runtime" or it won't work. (open it)
	// also in the data layers panel, set the initial runtime state.
	// this is ok to be a softobjectptr since they will be loaded during the loading of the step and the dl itself.
	// is up to you to set up the step in a way that this is not a breaking stuff (e.g. by using the fade).
	// it HAS to have the .XXXX at the end of the path. (e.g. Rooms/Room03.Room03) or it won't load.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	TArray<TSoftObjectPtr<UDataLayerAsset>> DL_Load; // TODO ensure this is not broken on packaged builds
	// Data layers to unload
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	TArray<TSoftObjectPtr<UDataLayerAsset>> DL_Unload; // TODO ensure this is not broken on packaged builds

	// set to true to use debug
	inline static bool Debug = false;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;
	void UpdateCamEnabled() const;
	void CamBlend();

	// these are callable so that can be reused by children.
	// they don't check for UseCamShake
	UFUNCTION(BlueprintCallable)
	void CamShakeStart();
	// these are callable so that can be reused by children.
	// they don't check for UseCamShake
	UFUNCTION(BlueprintCallable)
	void CamShakeStop();

	// don't call directly. called by system.
	// called when the step just starts. can be in the middle of a fade.
	// You should override Start instead, unless you know what you're doing.
	// one reason would be to do something just when the fade is on (like loading or fading something).
	// be sure not to do anything that would finish the step here, use Start for that (and even then use a timer for next tick).
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(AdvancedDisplay, ForceAsFunction))
	void TryStart() ;
	
	// don't call this one directly. called by the system.
	// Override to perform whatever the step needs to do at the end.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(ForceAsFunction))
	void Stop();

	// Will be triggered when the wait time ends.
	// if the WaitTime is <=0 it will be called next frame after parent start.
	// If FinishPostWait is set, it will finish the step.
	// this is a good place to do checks that might finish the step.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(ForceAsFunction))
	void Start();

	// gets called when UseDebug is set. happens on postwait
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(ForceAsFunction))
	void DoDebug();
	virtual void DoDebug_Implementation() {};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Cam = nullptr;

	friend class UStory;
};
