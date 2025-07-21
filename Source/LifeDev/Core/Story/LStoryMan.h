// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Core/Settings/FLChapter.h"
#include "Story/StoryMan.h"

#include "LStoryMan.generated.h"

class UDiags;
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

	// called by the gamemode on start
	UFUNCTION()
	void ChapStart();

protected:
	virtual void BeginPlay() override;
	void ChapStartEnd() const;
	UFUNCTION() // bind to delegate
	void ChapStartNext();
	bool ChapLoad();

	// default name of the end step
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category="SetUp")
	FName StepEndName="End";

#pragma region cache
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FLChapter Chapter;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ALGGameMode> GM = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<AGhostPool> Ghosts = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettings> Settings = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
#pragma endregion
};
