// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CAnimator.h"

#include "AnimTracks.generated.h"

// This is just experimental and can be removed at any point

// thanks to https://slowburn.dev/blog/polymorphic-serialization-in-unreal-engine/


UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class UAnimTrackBase : public UObject {
	GENERATED_BODY()

public:
	virtual void Update(float Progress, float Alpha) {};
};


UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class UOAnimTrackF : public UAnimTrackBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Start = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float End = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInstanceDynamic* Mat = nullptr;

	virtual void Update(float Progress, float Alpha) override {};
};

// USTRUCT(Blueprintable, BlueprintType)
// struct INTERACT_API FFAnimTrackBase {
// 	GENERATED_BODY()
//
// 	uint8 Type = 0;
// public:
// 	FFAnimTrackBase() { }
// 	~FFAnimTrackBase() {};
// 	void Update(float Alpha);
// };


USTRUCT(Blueprintable, BlueprintType)
struct INTERACT_API FAnimTrackMatF {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float Start = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float End = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UMaterialInstanceDynamic* Mat = nullptr;

	void Update(float Alpha);
};

USTRUCT(Blueprintable, BlueprintType)
struct INTERACT_API FAnimTrackMatV  {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FLinearColor Start = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FLinearColor End = FLinearColor::White;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UMaterialInstanceDynamic* Mat = nullptr;

	void Update(float Alpha);
};

USTRUCT(Blueprintable, BlueprintType)
struct INTERACT_API FAnimTrackTrans {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FTransform Start;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FTransform End;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool IsAdditive = true;
	
	void Update(float Alpha);
	void Init();
};

// An interactive actor that can have an animation
// You can set the tick interval to control the performance of this component
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimTracks : public UCAnimator {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp|Tracks")
	TArray<FAnimTrackMatF> MatFs;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp|Tracks")
	TArray<FAnimTrackMatV> MatVs;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp|Tracks")
	TArray<FAnimTrackTrans> Transfs;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category="SetUp|Tracks")
	TArray<UAnimTrackBase*> Track;

	
protected:
	virtual void BeginPlay() override;
	virtual void Update_Implementation(float Alpha) override;
};
