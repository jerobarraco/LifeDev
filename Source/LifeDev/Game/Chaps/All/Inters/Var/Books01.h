// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Var/Books.h"

#include "Books01.generated.h"


// base class for interactable books. used for puzzles.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooks01: public ABooks {
	GENERATED_BODY()

public:
	ABooks01();
protected:
	virtual void SetState_Implementation(const int32 NewState) override;
	virtual void Constructor() override;

	// cache the transforms to simplify the code
	TArray<FTransform> BookTrans;
};
