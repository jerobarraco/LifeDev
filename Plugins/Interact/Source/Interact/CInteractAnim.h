// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "CInteractAnim.generated.h"

class UCInteract;
class UCAnimatorTrans;
class UStaticMeshComponent;

// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCInteractAnim: public USceneComponent {
public:
	GENERATED_BODY()

	UCInteractAnim(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitProperties() override;
	virtual void PostCDOContruct() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool IsEnabled);

	UFUNCTION(BlueprintCallable)
	void SetText();
	
	// used by the interactor. don't call directly. subscribe to OnTrigger.
	UFUNCTION(BlueprintCallable)
	void Trigger();
	
	// used by the interactor. don't call directly. subscribe to the OnHover delegate.
	UFUNCTION()
	void Hover(bool IsHover) const;

	UFUNCTION()
	void DeInit();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TArray<FText> Texts = {
		FText::FromString(TEXT("Opened Text")), // isOpen
		FText::FromString(TEXT("Closed text")), // !IsOpen
	};
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool Locked = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCInteract* Interact = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* Root = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCAnimatorTrans* Animator = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh = nullptr;
protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsOpen = false;
};
