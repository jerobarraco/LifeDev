// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Diags/DiagMan.h"
#include "Inventory/Flags.h"

#include "LDiagMan.generated.h"

enum class EFeat : uint8;

// Dialog manager. dynamically instanced on the level
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALDiagMan : public ADiagMan {
	GENERATED_BODY()

public:
	ALDiagMan();

	// avoid using this. try to cache.
	// Not very fast.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALDiagMan* InstanceL(const UObject* const O);

	virtual void Init_Implementation() override;

	static constexpr float DefAutoTime = 2.5;
	// how much to wait before trying to auto skip.
	// Requires feature flag D_AUTO
	// a very low value can break stuff.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogs", Config, meta=(ClampMin=.05))
	float AutoTime = DefAutoTime;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Show_Implementation(const FDiag& Diag) override;
	// virtual void Add_Implementation(const FName Name, const FDiag& Diag) override;
	virtual void Hidden_Implementation() override;
	virtual void Back_Implementation() override;
	void AutoClear();
	UFUNCTION()
	void FeatUp(const EFeat Feat, const bool Enabled);

	UPROPERTY(BlueprintReadOnly, Category="Dialogs", VisibleAnywhere, Transient)
	bool UseAuto = false;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;

	FTimerHandle AutoTimer;
};