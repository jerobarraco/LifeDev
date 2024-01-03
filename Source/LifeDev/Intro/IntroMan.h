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
	FString NextLevel = "Game_L";

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void AddUI();
	
	UFUNCTION()
	void Done();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UIntroUI* UI = nullptr;
};
