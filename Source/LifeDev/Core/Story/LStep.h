// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Story/Step.h"
#include "Inventory/InventoryTypes.h"
#include "Diags/DiagTypes.h"

#include "LStep.generated.h"

class UCQuickMesh;
class UCAnimatorTrans;
class UCRandomizerFB;
class ALInteract;
class AInteract;
class AGhosts;
class UFlags;
class UFlashback;
class UDiags;
class UInventory;

// LifeDev base class for story steps
// be careful it will clear all timers on stop
UCLASS(Blueprintable, BlueprintType)
class ALStep: public AStep {
	GENERATED_BODY()

public:
	ALStep();

	virtual void TryStart_Implementation() override;

	// Be careful! this will remove all delegates to Dialogs.OnDone and Inventory.OnMOd
	// it will also remove from inventory the items on RemItems.
	// And it's possible this class will get unloaded by the next step.
	virtual void Stop_Implementation() override;

	// enables or disables the linked actors
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	void SetActorsShowActive(const bool Active = true, const bool WithFade=true);
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	void SetActorsHideActive(bool Active, bool WithFade=true);

	// enables or disables the interacts on IntersActiveAuto
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay, UnsafeDuringActorConstruction))
	void SetIntersActiveAuto(const bool NewActive=true);

	// activates the interacts on IntersActivate
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay, UnsafeDuringActorConstruction))
	void DoIntersActive();
	// disables the interacts on IntersDeactivate
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay, UnsafeDuringActorConstruction))
	void DoIntersDeactive();

	inline static bool UseDebug = false;

#pragma region dlgs
	// how much to mod the fb by on each dialog.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	float FBDlgMod = 0.0;
	// when true, it will calculate the FBDiagMod on Start, to reach FBDiagAutoTo
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	bool UseFBDlgAuto = false;
	// When UseFBDiagAuto is set, this is the target FB value used to calculate FBDiagMod.
	// calculated on Start
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	float FBDlgAutoTo = 1.0;	// whether to manage the RandFB. Enable at start, disable at stop.
	// will animate the camera using the AnimTarget as the target transform. it's bound to the flashback.
	// requires FBDlgAutoTo to be set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	bool UseFBAnim = false;
#pragma endregion

	// whether to dis/enable the character input
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	bool InputEnabled = false;
	// whether to manage the RandFB. Enable at start, disable at stop.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	bool UseFBRand = false;
	// whether to manage rain. Enable at start, disable at stop.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	bool UseRain = false;
	// whether to spawn (and destroy) ghosts. Take a look at and set GhostPos.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	bool UseGhosts = false;
	// relative position where the ghosts will spawn
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	FVector GhostPos;

#pragma region interacts
	// Actors to show AND hide. If it's an interact, it will fade in and out.
	// Use IntersEnable/FadeIn/FadeOut instead otherwise.
	// purposely an actor to have flexibility.
	// it will hide the actor on beginplay.
	// the hiding on stop is deprecated. use ActorsHide
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Inters")
	TArray<TSoftObjectPtr<AActor>> ActorsShow;
	// i don't move this to Step because the fade has a timing component before destroy
	// or maybe i could, if i leave the "destroy" only for LStep
	// but that would make it lame to use, as both classes would behave differently

	// Actors to Hide on *End*. If it's an Linteract, it will fade out. it will also set active.
	// TODO move code that relies on ActorsShow hiding actors at end to use this instead
	// TODO remove hiding actors from ActorsShow
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Inters")
	TArray<TSoftObjectPtr<AActor>> ActorsHide;
	
	// Interacts to activate on Start (after wait), and disable on Stop.
	// Will be deactivated on begin play.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<TSoftObjectPtr<AInteract>> IntersActivateAuto;

	// Interacts to activate on Start (after wait)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<TSoftObjectPtr<AInteract>> IntersActivate;

	// Interacts to deactivate on Start (after wait).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<TSoftObjectPtr<AInteract>> IntersDeactivate;

	// Interacts to trigger out during Start (after wait). Won't change fade during beginPlay.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<TSoftObjectPtr<ALInteract>> IntersTrigger;

	// Interacts to fade in during Start (post wait). Won't change fade during beginPlay.
	// Note: Fade also calls SetActive. It will also ensure is not SetHiddenInGame.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters", meta=(DeprecatedProperty))
	TArray<TSoftObjectPtr<ALInteract>> IntersFadeIn; // deprecated in favor of actorShow/Hide

	// Interacts to fade out during Start (post wait). Won't change fade during beginPlay.
	// Note: Fade also calls SetActive.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters", meta=(DeprecatedProperty))
	TArray<TSoftObjectPtr<ALInteract>> IntersFadeOut; // deprecated in favor of actorShow/Hide

	// Interacts to set UseHint at the Start (post wait).
	// Will not stop being hinted automatically.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<TSoftObjectPtr<AInteract>> IntersHint;
#pragma endregion

#pragma region rewards
	// it will advance once ALL flags are obtained. (it also waits for FinishItems)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Items")
	TArray<FName> FinishFlags;

	// if this is set. it will advance once ALL items are obtained.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Items")
	TArray<FName> FinishItems;

	// items to remove (completely) when the step *finishes*.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Items")
	TArray<FName> RemoveItems;

	// items to ensure when starting. mostly for skipping chapters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Items")
	TArray<FName> EnsureItems;
#pragma endregion

protected:
	virtual void Start_Implementation() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Finish_Implementation() override;
	void Unbind() const;
	// Sets the FBAutoMod based on the Id passed.
	// FBDlgAutoTo must be set first.
	void SetFBDlgAuto(const FName& Id);
	// will start the dialogs
	void StartDialogs();
	// will finish once the dialogs are done, or immediately if not showing.
	// safe to call multiple times.
	void FinishAfterDlgs();
	// on stop, removes items on RemItems
	void DoRemoveItems();
	// on start, adds the items on ItemsEnsure
	void DoEnsureItems();
	// will destroy some transient actors
	UFUNCTION()
	void DestroyActors();
	
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void DlgShow(const FDiag& Diag);
	// called when items get mod. checks for itemsFinish
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))// bound
	void ItemMod(const FName& ItemName, int32 Diff, const FItem& Item);
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void FlagMod(const FName& FlagName, const float Diff, const float Total);
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void FBUpd(const float Value);

	// checks if we have the itemsFinish and finish (after a dialog if any)
	// no need to call this as this class will watch the inventory. but just in case.
	UFUNCTION()
	void CheckFinish();
	bool HasItemsFinish();
	UFUNCTION()
	bool HasFlagsFinish();
	
	// trigger inters on IntersTrigger
	void DoIntersTrigger() const;
	void DoIntersHint() const;
	// fade an array of ALInteract
	static void DoIntersFade(const TArray<TSoftObjectPtr<ALInteract>>& SA, const bool In);
	static void DoIntersActiveAny(const TArray<TSoftObjectPtr<AInteract>>& A, const bool NewActive);

#pragma region cdo
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCRandomizerFB> RandFB = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCAnimatorTrans> Anim = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USceneComponent> AnimTarget = nullptr;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> AnimArrow = nullptr;
	// for editor previewing only. does not exists on packaged builds.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCameraComponent> AnimCam = nullptr;
#endif
#pragma endregion

#pragma region cache
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlashback> FB = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<AGhosts> Ghosts = nullptr;
#pragma endregion 
	FTimerHandle TimerDestroy;
	float FBFrom = 0; // a cache used to remap the fb values for fbanim
};

// it would be nice to have a AnimatorCam but i don't need it atm. otherwise look at ALNPC01I00
