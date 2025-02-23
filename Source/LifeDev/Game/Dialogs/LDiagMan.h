// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Diags/DiagMan.h"
#include "Inventory/Flags.h"

#include "LDiagMan.generated.h"

// Dialog manager. dynamically instanced on the level
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALDiagMan : public ADiagMan {
	GENERATED_BODY()

public:
	ALDiagMan();

	virtual void Init_Implementation() override;

	// avoid using this. try to cache.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALDiagMan* InstanceL(const UObject* const O);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetAutoTime() const { return AutoTime; }

	// how much to wait before trying to auto skip.
	// Requires feature flag D_AUTO
	// a very low value can break stuff.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogs", Config, meta=(ClampMin=.05))
	float AutoTime = 2.5;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Show_Implementation(const FDiag& Diag) override;
	virtual void DiagDone_Implementation() override;
	virtual void Back_Implementation() override;
	void AutoClear();

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;

	FTimerHandle AutoTimer;
};