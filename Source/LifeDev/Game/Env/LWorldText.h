// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/TextRenderComponent.h"

#include "LWorldText.generated.h"

// Base class for in-world texts.
// at some point think about making it an LInteract if i need to do stuff
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALWorldText: public AActor {
	GENERATED_BODY()

public:
	ALWorldText();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UTextRenderComponent> Root = nullptr;
};
