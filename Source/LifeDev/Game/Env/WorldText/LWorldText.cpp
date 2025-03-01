// Copyright Jerónimo Barraco-Mármol

#include "LWorldText.h"

#include "Components/TextRenderComponent.h"

ALWorldText::ALWorldText():Super() {
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXT"));
	SetRootComponent(Root);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Env/WorldText/WorldText_M"));
	Root->SetMaterial(0, CMat.Object);
	Root->SetWorldSize(25);
	Root->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Root->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	
	// Root->SetFont() // TODO
}

// TODO do smth with the fb?