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

	// sets the mobility. Override if you add more components that need to change.
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

	// Flip-flops depending on the state. Starts closed, then open, then closed.
	// this also controls the "reversed" flag of the anim.
	// If you need not to use the reversed animation then use the "Trans" property.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsClosed() const { return State % 2 == 0; }

	// Whether it should reverse the animation.
	// By default it will not reverse if StateNum <=1. otherwise it depends on IsClosed.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsReversed() { return StateNum > 1 && IsClosed(); }

	// whether it will trigger animations using the Anim component.
	// this gets abused in several situations. like when changing the mobility,
	// when strobe is disabled on lights, on beginplay, and many many more. :)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseAnim = true;

	// will disable while playing and re-enable after.
	// be careful if you need to disable on trigger or smth (for example Puzzle::DisableOnDone) 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool DisableWhileAnim = true;
	
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
	
	// The animator, by default set up for the mesh material and iroot
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCAnimatorMix* Anim = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetText_Implementation() override;
	virtual void SetState_Implementation(int32 NewState) override;
	virtual bool TryTrigger_Implementation() override;
	virtual void Trigger_Implementation() override;
	
	// triggers the animation. checks some flags first.
	void AnimPlay();

	// Called when the animation begins. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) // bound
	void AnimBegin();
	virtual void AnimBegin_Implementation();

	// Called when the animation end. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) // bound
	void AnimEnd();
	virtual void AnimEnd_Implementation();
};
