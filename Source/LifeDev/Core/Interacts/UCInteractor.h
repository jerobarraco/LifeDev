// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "UCInteractor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractStarts, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractEnd, AActor*, Actor);

class UBoxComponent;
// Will be interacting with interactables
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCInteractor: public USceneComponent {
public:
	GENERATED_BODY()

	UCInteractor(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TraceLen = 10.0;
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractStarts OnStart;
	UPROPERTY(BlueprintAssignable)
	FOnInteractEnd OnEnd;

private:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void DoEnd();
	void DoStart(AActor* Actor);
	UPROPERTY(Transient)
	AActor* InteractActor = nullptr;
};
