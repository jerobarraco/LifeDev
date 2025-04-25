// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Inventory/Teach/TeachMan.h"

#include "LTeachMan.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALTeachMan: public ATeachMan {
	GENERATED_BODY()
public:
#pragma region base
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALTeachMan* Instance(const UObject* const O);
	virtual void Init_Implementation(UDataTable* Data) override;
	virtual void DeInit_Implementation() override;
#pragma endregion

#pragma region delegates
#pragma endregion
protected:
	void DeInitInter();
	UFUNCTION()
	void InterTriger(const UCInteract* const Comp);
	UFUNCTION()
	void InterHover(const bool bOn, UCInteract* const Comp);
};
