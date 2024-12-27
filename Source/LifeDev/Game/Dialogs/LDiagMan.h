// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Diags/DiagMan.h"
#include "Inventory/Flags.h"

#include "LDiagMan.generated.h"

// Dialog manager. dynamically instanced on the level
UCLASS(Blueprintable, DefaultConfig, Config=LifeDev)
class LIFEDEV_API ALDiagMan : public ADiagMan {
	GENERATED_BODY()

public:
	ALDiagMan();

	// avoid using this. try to cache.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALDiagMan* InstanceL(const UObject* const O);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetAutoTime() const { return AutoTime; }

	// will also save the value to the settings
	UFUNCTION(BlueprintCallable)
	void SetAutoTime(const float NewTime);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Show_Implementation(const FDialog& Diag) override;
	virtual void DiagDone_Implementation() override;
	virtual void Back_Implementation() override;
	void AutoClear();

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogs", Config)
	float AutoTime = 2.5;

	FTimerHandle AutoTimer;
};