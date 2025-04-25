// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Inventory/Teach/TeachMan.h"

#include "LTeachMan.generated.h"

struct FDiag;
class AStep;
struct FItem;
class UCInteract;

UCLASS(Blueprintable, BlueprintType, Config=Inventory, DefaultConfig)
class LIFEDEV_API ALTeachMan: public ATeachMan {
	GENERATED_BODY()
public:
#pragma region base
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALTeachMan* Instance(const UObject* const O);
	virtual void Init_Implementation(UDataTable* Data) override;
	virtual void DeInit_Implementation() override;
	UFUNCTION()
	void InitDelayed(); // called when the game already started
#pragma endregion

	UPROPERTY(BlueprintReadWrite, Config)
	float InitDelayTime = 3;
#pragma region delegates
#pragma endregion
protected:
	void DeInitItemMod();
	void DeInitInter();
	void DeInitDiag();
	void DeInitStory();
	UFUNCTION()
	void ItemMod(const FName& Name, const int32 Diff, const FItem& Item);
	UFUNCTION()
	void InterTrigger(const UCInteract* const Comp);
	UFUNCTION()
	void InterHover(const bool bOn, UCInteract* const Comp);
	bool ItemHasAll();
	UFUNCTION()
	void DiagAdd(const FName& Name, const FDiag& Diag);
	UFUNCTION()
	void StepStart(AStep* const Step);
};
