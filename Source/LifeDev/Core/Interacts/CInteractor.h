// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/ArrowComponent.h"

#include "CInteractor.generated.h"

class UCInteract;
class UInteractorUI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractStarts, UCInteract*, Comp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractStop, UCInteract*, Comp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractToggle, bool, On, UCInteract*, Comp);

class UBoxComponent;
// Will be interacting with interact objects
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCInteractor: public USceneComponent {
public:
	GENERATED_BODY()

	UCInteractor(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void TryTrigger();
	
	// The max length to trace for
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TraceLen = 1000.0;
	// The class of the ui to use
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UInteractorUI> UIClass = nullptr;

	UPROPERTY(BlueprintAssignable)
	FOnInteractToggle OnToggle;
	UPROPERTY(BlueprintAssignable)
	FOnInteractStarts OnStart;
	UPROPERTY(BlueprintAssignable)
	FOnInteractStop OnStop;
	

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void SetUIVisible(bool Visible) const;
	// ends an interaction
	void DoEnd();
	// attempts to trigger a start
	void DoStart(UCInteract* Component);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UArrowComponent* Arrow = nullptr;

	UPROPERTY(Transient)
	UCInteract* InterComp = nullptr;
	UPROPERTY(Transient)
	UInteractorUI* UI = nullptr;
};
