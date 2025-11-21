// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"
#include "Diags/DiagTypes.h"

#include "Inventory/InventoryMan.h"
#include "LifeDev/Core/Consts/ConstSettings.h"
#include "LInventoryMan.generated.h"

UCLASS(Blueprintable)
class LIFEDEV_API ALInventoryMan : public AInventoryMan {
	GENERATED_BODY()

public:
	ALInventoryMan();
	virtual void Init_Implementation() override;
	virtual void DeInit_Implementation() override;

protected:
	UFUNCTION()
	void FeatUp(const EFeat Feat, const bool Enabled);
	UFUNCTION()
	void DiagShow(const FDiag& Diag) {ToggleInput(false);}
	UFUNCTION()
	void DiagDone() { ToggleInput(true); }
};
