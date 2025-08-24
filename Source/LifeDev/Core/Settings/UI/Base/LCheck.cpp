// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "LCheck.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

ULCheck::ULCheck(const FObjectInitializer& O):UUserWidget(O) {
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CSC(TEXT("/Game/LifeDev/Core/UI/LCheck_S"));
	StyleCheck = CSC.Object;
	
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CST(TEXT("/Game/LifeDev/Core/UI/Btns/LTextSmall_S"));
	StyleText = CST.Object;
}

void ULCheck::Apply_Implementation() {}

void ULCheck::Load_Implementation() {}

void ULCheck::ResetStyle() {
	if (bool(StyleCheck) & bool(Check)) {
		// const FButtonStyle* const Style = StyleCheck->GetStyle<FButtonStyle>();
		// if (LIKELY(Style)) Style(*Style);
		const FCheckBoxStyle* const S = StyleCheck->GetStyle<FCheckBoxStyle>();
		if (LIKELY(S)) Check->SetWidgetStyle(*S);
	}

	if (bool(StyleText) & bool(Text)) {
		const FTextBlockStyle* const Style = StyleText->GetStyle<FTextBlockStyle>();
		// because ue doesn't expose it :'(
		if (LIKELY(Style)) {
			//Text->style StyleSetTextStyle(*Style); {
			// this is a bit hackish. but ue doesn't provide a proper way.
			// SWidget* const SW = &Text->TakeWidget().Get();
			// STextBlock* const STB = static_cast<STextBlock*>(SW);
			// if (STB)
				// STB->SetTextStyle(Style);
			//
			// Text->SetText( St);
			// Text->SetFont( Font );
			// Text->SetStrikeBrush( &StrikeBrush );
			// Text->SetColorAndOpacity( ColorAndOpacityBinding );
			// Text->SetShadowOffset( ShadowOffset );
			// Text->SetShadowColorAndOpacity( ShadowColorAndOpacityBinding );
			// Text->SetMinDesiredWidth( MinDesiredWidth );
			// Text->SetTransformPolicy( TextTransformPolicy );
			// Text->SetOverflowPolicy(TextOverflowPolicy);

			
		}
	}
}

void ULCheck::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}
