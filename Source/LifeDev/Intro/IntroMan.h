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

	UFUNCTION()
	void SlotsDone(const bool HasDoneSave);
	void AddUI();
	
	UFUNCTION()
	void Done();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UIntroUI> UI = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> MusicNew = TSoftObjectPtr<USoundBase>(
		FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music09/Music09_MS.Music09_MS"))
	);
};
