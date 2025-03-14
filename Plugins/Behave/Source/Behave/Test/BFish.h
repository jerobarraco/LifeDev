// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "Behave/BTypes.h"

#include "BFish.generated.h"

class UBBase;
class UCBehave;
class UTextRenderComponent;
class UCQuickMesh;

USTRUCT(Blueprintable, BlueprintType)
struct FTFishData {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Tired = 0;
};

// this is just a test. warning
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Behave), meta=(BlueprintSpawnableComponent))
class BEHAVE_API ABFish: public AActor {
	GENERATED_BODY()
public:
	ABFish();

protected:
	UFUNCTION()
	void ActStateUp(UBBase* const Act, const EBState State);
	
	UFUNCTION()
	bool CanEat();
	UFUNCTION()
	void MoveToFood();

	UFUNCTION()
	void MoveToSleep();
	UFUNCTION()
	void MoveToPlay();
	virtual void BeginPlay() override;
	virtual void Tick(const float DT) override;

	UFUNCTION()
	void UpdBio(UBBase* const Behave);
	UFUNCTION()
	void UpdEmo(UBBase* const Behave);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Body;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Tail;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USceneComponent> Stats;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> S_Tired;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> S_Hungry;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> S_Bore;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UTextRenderComponent> Text = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCBehave> Behave;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FTFishData Data;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName Doing;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config)
	float MoveSpeed = 30;

	FVector MoveTgt;
	bool Moving = false;
};
