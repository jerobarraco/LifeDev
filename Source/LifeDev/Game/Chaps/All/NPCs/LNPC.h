// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LNPC.generated.h"

class UMaterialInterface;

// base npc class
UCLASS(Blueprintable, BlueprintType)
class ALNPC : public ALInteract {
	GENERATED_BODY()

public:
	ALNPC();

	UFUNCTION(BlueprintCallable)
	void SetFadeMat(UMaterialInterface* NewMat = nullptr);
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Fade(bool In);
	UFUNCTION(BlueprintCallable, CallInEditor)
	void FadeIn() {Fade(true);};
	UFUNCTION(BlueprintCallable, CallInEditor)
	void FadeOut(){Fade(false);};
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	UMaterialInterface* FadeMatClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Transient)
	UMaterialInstanceDynamic* FadeMat = nullptr;
};
