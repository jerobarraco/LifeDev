// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "LBLogItemW.h"
#include "Diags/DiagTypes.h"

#include "LifeDev/Core/Settings/UI/LSetBaseUI.h"

#include "LBLogW.generated.h"

// currently shown on the settings menu, but later will be moved somewhere else maybe.

class UScrollBox;
class ULBLogItemW;
// a stub to show the backlog of dialogs.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULBLogW: public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override {};
	virtual void Load_Implementation() override {};
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void DiagAdd(const FName Name, const FDialog& Diag);
	UFUNCTION()
	void DiagSpace();

	TArray<FName> Seen;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Setup")
	TSubclassOf<ULBLogItemW> ItemClass = ULBLogItemW::StaticClass();

	// TODO real class
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Setup")
	TSubclassOf<UUserWidget> SpacerClass = ULBLogItemW::StaticClass();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> Scroller = nullptr;
};
