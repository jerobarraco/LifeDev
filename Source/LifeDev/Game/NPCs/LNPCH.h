// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LNPC.h"

#include "LNPCH.generated.h"

USTRUCT(BlueprintType)
struct FNPCHPose {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector InteractOrg;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector InteractExt;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Pelvis;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Torso;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Head;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmL1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmL2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmR1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmR2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegL1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegL2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegR1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegR2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform FootL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform FootR;
};

// Base class for human-like npcs
UCLASS(Blueprintable, BlueprintType)
class ALNPCH : public ALNPC{
	GENERATED_BODY()

public:
	ALNPCH();

	UFUNCTION(BlueprintCallable)
	void SetPose(const FNPCHPose& Pose);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FNPCHPose PoseBase;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Head;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Torso;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Pelvis;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* ArmL1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* ArmR1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* ArmL2;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* ArmR2;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* LegL1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* LegR1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* LegL2;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* LegR2;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* FootL;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* FootR;
};
