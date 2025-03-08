// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSaveGroup.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Inventory/Flags.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

void ULSaveGroup::LoadDoneAll_Implementation() {
	
}

void ULSaveGroup::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(SL_Foxy))
		SL_Foxy->OnValueChanged.AddUniqueDynamic(this, &ULSaveGroup::FoxyUpd);
}

void ULSaveGroup::FoxyUpd(const float Value) {
	if (UNLIKELY(!SL_Foxy || !T_Foxy)) return;

	T_Foxy->SetText(
		FText::FromString(FString::Printf(TEXT("%.3f"), Value)));
	UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;
	
	Flags->Set(LDConsts::Flags::Settings::Global::Foxy, Value);
}
