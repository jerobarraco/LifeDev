#pragma once

#include "CoreMinimal.h"
#include "LInteract.h"

#include "LInteractFade.generated.h"

class UCAnimatorFade;

// An interactive actor that has a fade animation, it triggers automatically at reward
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteractFade: public ALInteract {
public:
	GENERATED_BODY()

	ALInteractFade();
	
protected:
	UFUNCTION()
	void FadeDone();
	virtual void ItemRewarded_Implementation() override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="SetUp")
	UCAnimatorFade* AnimFade = nullptr;
};
