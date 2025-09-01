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
	FName Label = NAME_None;

	// Title to show. Only shown if you also set UseFade
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transition")
	FText Title = FText::GetEmpty();

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base", AssetRegistrySearchable, meta=(MultiLine))
	FString Comment;
#endif

	// Actor holding the camera to blend to. Or null to not use blend.
	// By default, it's this own step. or you can set UsePawnCam to use the pawn's camera.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam")
	TSoftObjectPtr<AActor> CamTarget = nullptr;

	// will target pawn automatically, will override the camTarget
	// this allows to blend TO the pawn camera (not the current cam)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam")
	bool UsePawnCam = false;
	// time used to animate the change of cameras. Only where CamTarget is valid
	// blend and fade could have some issues when used together.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam", Config)
	float CamBlendTime = 2;
	// shake the camera on start. stops on stop.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Cam")
	bool UseCamShake = false;
	// class that defines the shake. depends on UseCamShake.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp|Cam")
	TSubclassOf<UCameraShakeBase> CamShakeClass = nullptr;

	// >0 will set the seconds to wait since the TryStart of this step. will trigger Start (override it).
	// This ONLY happens on Start. This affects the dialogs (the main usage). See UseFadeTime.
	// if a cam blend needs to happen, the wait will be clamped to _at least_ CamBlendTime.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transition")
	float WaitTime = 0;

	// When set to true, it will fade in/out using the game fade time on start.
	// blend and fade could have some issues when used together.
	// when set to true, loads will be awaited and will block the fade.
	// loads with UseFade=false will not block.
	// blocks require the DataLayers to be set to block. read note in DL_Load.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transition")
	bool UseFade = false; // see note inside Start()

	// teleports the character to where this cam is located. and also copies the rotation
	// beware this affects the camera blend if any (if blending from/to the character)
	// teleports before blending. teleports on start of step. Rotation is broken.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	bool TeleportChar = false;

	// Actors to show AND hide. If it's an interact, it will fade in and out.
	// Use IntersEnable/FadeIn/FadeOut instead otherwise.
	// purposely an actor to have flexibility.
	// it will hide the actor on BeginPlay.
	// the hiding on stop is deprecated. use ActorsHide
	// the hiding on begin play is deprecated. set on the class maybe.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Inters")
	TArray<TSoftObjectPtr<AActor>> ActorsShow;
	// not on AStep because i want to support fading as well, or maybe i should have the fade in LInteract::SetActorHiddenInGame? maybe i'll need a bool to tell if it needs to fade or not?

	// Actors to Hide on *End*. If it's a Linteract, it will fade out. it will also set active and hidden in game.
	// a hidden benefit from this is that i can hide things on the last step in a chapter
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Inters")
	TArray<TSoftObjectPtr<AActor>> ActorsHide;

	// Music to play. Setting it to empty does not stop the music.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	TSoftObjectPtr<USoundBase> Music = nullptr;
	// TODO fix not packaging*

	// Data layers to load.
	// make sure to mark the data-layer-asset as "runtime" or it won't work. (open it)
	// also in the data layers panel, set the initial runtime state.
	// also make sure to set the "Override blocking on slow stream" (under advanced)
	// to whatever you need. Set it to block if you want it to block, otherwise block has no effect.
	// this is ok to be a SoftObjectPtr. They will be loaded during the loading of the step and the dl itself.
	// is up to you to set up the step in a way that this is not a breaking stuff (e.g. by setting UseFade or ensuring it looks ok).
	// it HAS to have the .XXXX at the end of the path. (e.g. Rooms/Room03.Room03) or it won't load.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	TArray<TSoftObjectPtr<UDataLayerAsset>> DL_Load;
	// Data layers to unload
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	TArray<TSoftObjectPtr<UDataLayerAsset>> DL_Unload;

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
	// called when the step just starts. it can be in the middle of a fade.
	// You should override Start instead, unless you know what you're doing.
	// one reason would be to do something just when the fade is on (like loading or fading something).
	// be sure not to do anything that would finish the step here, use Start for that (and even then use a timer for next tick).
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(AdvancedDisplay, ForceAsFunction))
	void TryStart();
	
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
