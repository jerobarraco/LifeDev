// Copyright Jerónimo Barraco-Mármol 2025

#pragma once
#include "Components/TextRenderComponent.h"

#include "LWorldText.generated.h"

// i thought on using a decal, but docs says it works only on static objects (which was a lie)
// but its a risk https://dev.epicgames.com/documentation/en-us/unreal-engine/decal-actors-in-unreal-engine
// but also rendering text on a material seems difficult. and i need to create an actor ANYWAY

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
