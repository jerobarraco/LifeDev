// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "AnimTracks.generated.h"

// This is just experimental and can be removed at any point

UCLASS(Blueprintable, BlueprintType)
class UAnimTrackBase : public UObject {
	GENERATED_BODY()

public:
	virtual void Update(float Progress, float Alpha);
};


UCLASS(Blueprintable, BlueprintType)
class UAnimTrackMatF : public UAnimTrackBase {
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

	virtual void Update(float Progress, float Alpha) override;
};
//
// USTRUCT(Blueprintable, BlueprintType)
// struct FFAnimTrackBase {
// 	GENERATED_BODY()
//
// 	uint8 Type = 0;
// public:
// 	virtual void Update(float Alpha);
// 	virtual ~FFAnimTrackBase(){};
// };
//
//
// USTRUCT(Blueprintable, BlueprintType)
// struct FFAnimTrackMatF : public FFAnimTrackBase {
// 	GENERATED_BODY()
//
// public:
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
// 	FName Name;
//
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
// 	float Start = 0.0;
// 	
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
// 	float End = 1;
//
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
// 	UMaterialInstanceDynamic* Mat = nullptr;
//
// 	virtual void Update(float Alpha) override;
// 	virtual ~FFAnimTrackMatF() override {} ;
// };
