// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Books.h"

#include "Books01.generated.h"

// base class for interactable books. used for puzzles. defaults to 3 books.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooks01: public ABooks {
	GENERATED_BODY()

public:
	ABooks01();

protected:
	virtual void SetState_Implementation(const int32 NewState) override;
	virtual void BeginPlay() override;
	
	// cache the transforms to simplify the code
	TArray<FTransform> BookTrans;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCurveFloat> FadeCurve = nullptr;

private:
	void ConstructB01();
};
