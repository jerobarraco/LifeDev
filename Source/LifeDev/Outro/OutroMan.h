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
	FString GameLevel = "Intro_L";
	// "Game_L"; don't go directly to game given the savegame will be finished

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void BeginPlayPlus();
	
	void AddUI();
	void Quit();
	void Retry();

	UFUNCTION()
	void Done(int32 RetVal);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UOutroUI* UI = nullptr;
};
