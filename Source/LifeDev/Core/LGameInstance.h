// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "Engine/GameInstance.h"

#include "LGameInstance.generated.h"

class UUserWidget;

// LifeDev GameInstance
UCLASS(Blueprintable)
class LIFEDEV_API ULGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ULGameInstance* Instance(const UObject* const O);

	virtual void Init() override;

protected:
	virtual void Shutdown() override;
	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* const LoadedWorld);
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> LoadScreen = nullptr;

	bool CloseTriggered = false;
};
