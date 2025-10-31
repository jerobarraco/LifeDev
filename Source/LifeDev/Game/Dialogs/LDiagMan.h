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

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Show_Implementation(const FDiag& Diag) override;
	UFUNCTION()
	void FeatUp(const EFeat Feat, const bool Enabled);

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
};