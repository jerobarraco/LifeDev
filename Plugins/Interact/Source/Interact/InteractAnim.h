// Copyright (C) 2023 - Jerónimo Barraco-Mármol
#pragma once
#include "Interact/Interact.h"

#include "InteractAnim.generated.h"

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API AInteractAnim: public AInteract {
public:
	GENERATED_BODY()

	AInteractAnim();
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Close")), // isOpen // Opened text
		FText::FromString(TEXT("Open")), // !IsOpen // Closed text
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	USoundBase* SFX_Open = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	USoundBase* SFX_OpenEnd = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	USoundBase* SFX_Close = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	USoundBase* SFX_CloseEnd = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void SetText_Implementation() override;
	virtual void Trigger_Implementation() override;
	virtual bool TryTrigger_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent) // bound
	void AnimBegin();
	virtual void AnimBegin_Implementation();
	UFUNCTION(BlueprintNativeEvent) // bound
	void AnimEnd();
	virtual void AnimEnd_Implementation();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimatorTrans* Animator = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* SFX = nullptr;

	// starts closed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsOpen = false;
};
