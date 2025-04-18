// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "LOverlayUI.h"

#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "Interact/Animator/Anim.h"

#include "LifeDev/Core/Settings/LSettings.h"

ULOverlayUI::ULOverlayUI() :Super(){
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>
		CSaveMat(TEXT("/Game/LifeDev/Game/Dialogs/UI/Save/Saving_MI.Saving_MI"));
	SaveMat = CSaveMat.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Niagara/DefaultAssets/Curves/Templates/RampUpDown.RampUpDown"));
	SaveCurve = CCurve.Object;
}

void ULOverlayUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	ULSettings* const Settings = ULSettings::Instance(this);
	if (UNLIKELY(!Settings)) return;
	
	if (LIKELY(ISaving && SaveMat)) {
		SaveMatDyn = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, SaveMat);
		ISaving->SetBrushFromMaterial(SaveMatDyn);
	}

	Settings->OnSaving.AddUniqueDynamic(this, &ULOverlayUI::SaveUpd);
}


void ULOverlayUI::SaveUpd(const bool NewIsSaving) {
	IsSaving = NewIsSaving;

	if (!IsSaving) return; // don't call the timer on save done. let that happen naturally.
	SaveTimer();
}

void ULOverlayUI::SaveTimer() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	if (!IsSaving) {
		ISaving->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	ISaving->SetVisibility(ESlateVisibility::Visible);
	UAnim* const Anim = UAnim::Instance(this);
	if (LIKELY(Anim)) {
		constexpr const TCHAR* const PName = TEXT("Progress");
		FAParams P {.Name = PName, .Duration =  0};
		Anim->DynFloatFade(SaveMatDyn, P, 0);
		P.Duration = SaveTime;
		Anim->DynFloatFade(SaveMatDyn, P, 1);
	}
	UE_CLOG(!SaveMatDyn, LogTemp, Warning, TEXT("%hs SaveMatDyn null!"), __func__);

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ULOverlayUI::SaveTimer, SaveTime);
}
