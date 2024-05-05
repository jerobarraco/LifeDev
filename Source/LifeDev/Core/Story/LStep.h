// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Story/Step.h"
#include "Inventory/InventoryTypes.h"
#include "Diags/DiagTypes.h"

#include "LStep.generated.h"

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
	virtual void TryStart_Implementation() override;
	// careful this will clear all timers
	// and remove all delegates to Dialogs.OnDone and Inventory.OnMOd
	// it will also remove from inventory the items on RemItems
	virtual void Stop_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DlgShow(const FDialog& Diag);
	virtual void DlgShow_Implementation(const FDialog& Diag);

	// checks if we have the itemsFinish and finish (after a dialog if any)
	// no need to call this as this class will watch the inventory. but just in case.
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	void CheckItemsFinish();

	// enables or disables the linked actor
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay, DeprecatedFunction))
	void SetShowActorEnabled(const bool Enabled = true, const bool WithFade=true);

	// enables or disables the linked actors
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	void SetShowActorsEnabled(const bool Enabled = true, const bool WithFade=true);
	
	// enables or disables the interacts on IntersEnable
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	void SetIntersEnabled(const bool Enabled=true);

	// whether to dis/enable the character input
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool InputEnabled = false;

	// when set to true, the game mode will set the wait time to the fade time.
	// See WaitTime and FinishPostWait
	// this is used to start something AFTER it faded
	// don't use for fade out, since waitTime doesn't work with that, Stop is called just when the story faded out.
	// TODO make sure i don't need it and remove
	// TODO need to fix the steps that do use doStart
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Wait", meta=(DeprecatedProperty))
	bool UseFadeTime = false;

	// how much to mod the fb by on each dialog.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	float FbDiagMod = 0.0;
	
	// dialog or sequence to trigger on start.
	// This will make the step finish when the dialog finishes.
	// it will also disable FinishPostWait.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	FName DlgId = NAME_None;

	// whether to manage rain. Enable at start, disable at stop.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	bool UseRain = false;
	// whether to spawn (and destroy) ghosts. Take a look at and set GhostPos.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	bool UseGhosts = false;
	// relative position where the ghosts will spawn
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Extras")
	FVector GhostPos;

	// actor to show/hide. If it's an interact, it will fade in and out.
	// it will get destroyed on Stop, set this to null to avoid it.
	// purposely an actor to have flexibility
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Inters", meta=(DeprecatedProperty))
	AActor* ShowActor = nullptr;
	// i don't move this to Step because the fade has a timing component before destroy
	// or maybe i could if i leave the "destroy" only for LStep
	// but that would make it lame to use, as both classes would behave differently

	// Actors to show AND hide. If it's an interact, it will fade in and out.
	// It will get destroyed on Stop. Use IntersEnable/FadeIn/FadeOut instead otherwise.
	// purposely an actor to have flexibility
	// TODO port from ShowActor (WARNING i need to port all the objects that still use showactor)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Inters")
	TArray<AActor*> ActorsShow;
	// i don't move this to Step because the fade has a timing component before destroy
	// or maybe i could if i leave the "destroy" only for LStep
	// but that would make it lame to use, as both classes would behave differently
	
	// Interacts to enable on Start (after wait), and disable on Stop.
	// Will be disabled on begin play.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<AInteract*> IntersEnable;

	// Interacts to trigger out during Start (after wait). Won't change fade during beginPlay.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<ALInteract*> IntersTrigger;

	// Interacts to fade in during Start (post wait). Won't change fade during beginPlay.
	// Note Fade also calls SetEnabled. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<ALInteract*> IntersFadeIn;

	// Interacts to fade out during *Stop*. Won't change fade during beginPlay.
	// Note Fade also calls SetEnabled. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Inters")
	TArray<ALInteract*> IntersFadeOut;
	// note: not fading the intersFade* on begin play because i could have multiple
	// steps that collide with each other. not doing it on the intersEnable because i think
	// it won't happen. but it likely will. and when it does. i'll change it.
	// i hope i will remember.
	// TODO: make the IntersFadeout fade on start and not end.
	// that requires fixing the design of StepC4S*** 
	
	// if this is set. it will advance once ALL items are obtained.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Items")
	TArray<FName> ItemsFinish;

	// items to remove when the step finishes
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Items")
	TArray<FName> ItemsRem;

	// items to ensure when starting. mostly for skipping chapters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Items")
	TArray<FName> ItemsEnsure;

	inline static bool UseDebug = false;

protected:
	virtual void Start_Implementation() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;
	virtual void Finish_Implementation() override;
	
	// will start the dialogs
	void StartDialogs();
	// will finish once the dialogs are done, or immediately if not showing.
	// safe to call multiple times.
	void FinishAfterDlgs();
	// on stop, removes items on RemItems
	void RemoveItems();
	// on start, adds the items on ItemsEnsure
	void EnsureItems();
	// will destroy some transient actors
	UFUNCTION()
	void DestroyActors();
	// called when items get mod. checks for itemsFinish
	UFUNCTION()// bound
	void ItemMod(const FName& ItemName, int32 Diff, const FItem& Item);
	// fade an array of ALInteract
	void DoIntersFade(const TArray<ALInteract*>& A, const bool In);
	// trigger inters on IntersTrigger
	void DoIntersTrigger() const;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UDiags* Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UFlashback* FB = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UFlags* Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	AGhosts* Ghosts = nullptr;
};
