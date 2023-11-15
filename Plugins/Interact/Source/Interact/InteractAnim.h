// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "Interact/Interact.h"

#include "InteractAnim.generated.h"

class UCAnimatorMix;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType, Category="Interact|InteractAnim")
class INTERACT_API AInteractAnim: public AInteract {
	GENERATED_BODY()

public:
	AInteractAnim();
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

	// Flip-flops depending on the state. Starts closed. Override in case of State > 2.
	// this also affects the "reversed" flag of the anim. if you need something else set the "Trans".
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOpen() const { return State % 2 != 0; }
	
	// whether it will trigger animations from the Anim component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool AnimEnabled = true;

	// will disable while playing and re-enable after.
	// be careful if you need to disable on trigger or smth (for example Puzzle::DisableOnDone) 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool DisableWhileAnim = true;

	// Number of states. it will wrap around.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	int32 StateNum = 2;
	
	// Text to be displayed on each state. Closed, Open
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Open")), // 0 == !IsOpen == Closed text
		FText::FromString(TEXT("Close")), // 1 == IsOpen == Opened text
	};

	// mostly used for puzzles. i'm unsure i will keep this.
	// the transforms for each state. if this is set it will override the anim values.
	// the isAdditive flag will be respected, but probably won't work nicely.
	// won't affect the reversed flag.
	// if you need to set material values i'd recommend overriding SetState_Implementation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp", meta=(DeprecatedProperty))
	TArray<FTransform> Trans;

	// triggered when anim starts. Closed, open.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	TArray<USoundBase*> SFX_Start;
	// triggered when anim ends. Closed, Open.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	TArray<USoundBase*> SFX_Stop;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetText_Implementation() override;
	void AnimPlay();
	virtual void SetState_Implementation(int32 NewState) override;
	virtual bool TryTrigger_Implementation() override;
	virtual void Trigger_Implementation() override;

	// Called when the animation begins. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) // bound
	void AnimBegin();
	virtual void AnimBegin_Implementation();

	// Called when the animation end. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) // bound
	void AnimEnd();
	virtual void AnimEnd_Implementation();

	// The animator, by default set up for the mesh material and iroot
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimatorMix* Anim = nullptr;
};
