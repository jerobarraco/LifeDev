// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "IntroGameMode.generated.h"

class AIntroMan;
class ALMusicMan;

// Game mode class for the intro 
UCLASS(Blueprintable)
class LIFEDEV_API AIntroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIntroGameMode();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	USoundBase* Music = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient)
	AIntroMan* Manager = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient)
	ALMusicMan* MusicMan = nullptr;
};
