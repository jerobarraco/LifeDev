// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "LGameInstance.generated.h"

class ULSave;

// LifeDev GameInstance
UCLASS(Blueprintable)
class LIFEDEV_API ULGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static ULGameInstance* Instance(UWorld* World);

	virtual void Init() override;

protected:
	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	UPROPERTY(Transient)
	UUserWidget* LoadScreen = nullptr;
};
