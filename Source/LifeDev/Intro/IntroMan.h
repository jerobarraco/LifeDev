// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"
#include "IntroUI.h"

#include "IntroMan.generated.h"

UCLASS(Blueprintable)
class LIFEDEV_API AIntroMan : public AActor {
	GENERATED_BODY()

public:
	AIntroMan();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UIntroUI> UIClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,  Category=SetUp)
	// FString NextLevel = "/Game/LifeDev/Game/Chaps/All/L_Game";
	FString NextLevel = "L_Game";
	// need to define it under project settings > map & modes > advanced > gamemode aliases
	UPROPERTY(BlueprintReadWrite, EditAnywhere,  Category=SetUp)
	FString NextLevelMode = "LDG";

protected:
	virtual void BeginPlay() override;
	void AddUI();
	UFUNCTION()
	void Done();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UIntroUI* UI = nullptr;
};
