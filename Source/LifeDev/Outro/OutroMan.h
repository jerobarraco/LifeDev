// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"

#include "OutroMan.generated.h"

class UOutroUI;

UCLASS(Blueprintable)
class LIFEDEV_API AOutroMan : public AActor {
	GENERATED_BODY()

public:
	AOutroMan();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UOutroUI> UIClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,  Category=SetUp)
	FString GameLevel = "Game_L";

protected:
	virtual void BeginPlay() override;
	void AddUI();

	UFUNCTION()
	void Quit();
	
	UFUNCTION()
	void Done();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UOutroUI* UI = nullptr;
};
