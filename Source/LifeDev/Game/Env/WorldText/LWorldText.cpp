// Copyright Jerónimo Barraco-Mármol

#include "LWorldText.h"

#include "Engine/Font.h"
#include "Components/TextRenderComponent.h"

ALWorldText::ALWorldText():Super() {
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXT"));
	SetRootComponent(Root);
	Root->SetWorldSize(55); // minimum size that can be read, very close.
	Root->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Root->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Root->TextRenderColor = FColor::Black; // contrast
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Env/WorldText/WorldFBText_M"));
	Root->SetMaterial(0, CMat.Object);
	static ConstructorHelpers::FObjectFinder<UFont>
		CFont(TEXT("/Game/LifeDev/Game/Dialogs/UI/Text/Fonts/Offline/Rosemary_Roman"));
	Root->SetFont(CFont.Object);
	
	// Root->SetFont() // TODO
}

// TODO do smth with the fb?