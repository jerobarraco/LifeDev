// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/LInteract.h"
#include "Flames.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AFlames : public ALInteract {
	GENERATED_BODY()

public:
	AFlames();

protected:
	virtual void SetState_Implementation(const int32 NewState) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	TArray<TObjectPtr<UNiagaraComponent>> Flames;
};
