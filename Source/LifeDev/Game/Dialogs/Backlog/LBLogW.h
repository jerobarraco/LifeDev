// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "Diags/DiagTypes.h"

#include "LifeDev/Core/Settings/UI/LSetBaseUI.h"

#include "LBLogW.generated.h"

// currently shown on the settings menu, but later will be moved somewhere else maybe.

class ULBLogSpacerW;
class UScrollBox;
class ULBLogItemW;

// a stub to show the backlog of dialogs.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULBLogW: public ULSetBaseUI {
	GENERATED_BODY()

public:
	ULBLogW();
	virtual void Apply_Implementation() override {};
	virtual void Load_Implementation() override {};
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void Show_Implementation() override;
	
protected:
	UFUNCTION()
	void DiagAdd(const FName& Name, const FDiag& Diag);
	UFUNCTION()
	void DiagSpace();
	UFUNCTION()
	void DiagDone();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Setup")
	TSubclassOf<ULBLogItemW> ItemClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Setup")
	TSubclassOf<ULBLogSpacerW> SpacerClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Setup")
	float AnimTime = .5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> Scroller = nullptr;

	//HAS to be transient, or it will not compile the bp
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> AnimText = nullptr;

	TArray<FName> Seen;
	bool NeedsSpacer = false;
};
