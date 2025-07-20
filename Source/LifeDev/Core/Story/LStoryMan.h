// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Story/StoryMan.h"

#include "LStoryMan.generated.h"

class ALGGameMode;
class AGhostPool;
class ULSettings;
// Lifedev version of the Story manager
UCLASS(Blueprintable, BlueprintType, ClassGroup=(LifeDev), Config=LifeDev, DefaultConfig,
	meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ALStoryMan: public AStoryMan {
	// put the generic things into the baseclass
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALStoryMan* Instance(const UObject* const O);

	ALStoryMan();

	virtual void Init_Implementation() override;
	virtual void DeInit_Implementation() override;
	
	void ChapStartEnd() const;
	UFUNCTION() // bind to delegate
	void ChapStartNext();
	UFUNCTION() // bind to delegate
	void ChapStart();

protected:
	virtual void BeginPlay() override;

	// default name of the end step
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category="SetUp")
	FName StepEndName="End";

#pragma region cache
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ALGGameMode> GM = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<AGhostPool> Ghosts = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettings> Settings = nullptr;
#pragma endregion
};
