// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Diags/DiagMan.h"
#include "Inventory/Flags.h"

#include "LDiagMan.generated.h"

// Dialog manager. dynamically instanced on the level
UCLASS(Blueprintable)
class LIFEDEV_API ALDiagMan : public ADiagMan {
	GENERATED_BODY()

public:
	ALDiagMan();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Show_Implementation(const FDialog& Diag) override;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
};