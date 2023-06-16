// Copyright (C) 2023 - Jerónimo Barraco-Mármol
#pragma once
#include "Interact/Interact.h"

#include "InteractAnim.generated.h"

class UCAnimatorMix;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteractAnim: public AInteract {
public:
	GENERATED_BODY()

	AInteractAnim();

	// whether it will trigger animations from the Anim component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool AnimEnabled = true;

	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Open")), // !IsOpen // Closed text
		FText::FromString(TEXT("Close")), // isOpen // Opened text
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Open = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_OpenEnd = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Close = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_CloseEnd = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	USoundBase* SFX_Locked = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetText_Implementation() override;
	virtual bool TryTrigger_Implementation() override;
	virtual void Trigger_Implementation() override;
	virtual void TriggerLocked_Implementation() override;

	// Called when the animation begins. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) // bound
	void AnimBegin();
	virtual void AnimBegin_Implementation();
	// Called when the animation end. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) // bound
	void AnimEnd();
	virtual void AnimEnd_Implementation();

	UFUNCTION(BlueprintCallable)
	void PlaySFX(USoundBase* Snd);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimatorMix* Anim = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* SFX = nullptr;

	// starts closed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Lock")
	bool IsOpen = false;
};
