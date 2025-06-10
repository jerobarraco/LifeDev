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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	UFUNCTION()
	void BeginPlayPlus();
	UFUNCTION()
	void SlotsDone(const bool HasDoneSave);
	UFUNCTION()
	void Saving(const bool IsSaving);
	void AddUI();

	UFUNCTION()
	void Done();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UIntroUI> UI = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TSoftObjectPtr<USoundBase> MusicNew = TSoftObjectPtr<USoundBase>(
		FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music00/Music00_MS.Music00_MS")));
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TSoftObjectPtr<USoundBase> Music = TSoftObjectPtr<USoundBase>(
		FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music09/Music09_MS.Music09_MS")));
};
