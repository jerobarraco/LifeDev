// Copyright Jerónimo Barraco-Mármol

#include "LWorldText.h"

#include "Components/TextRenderComponent.h"

ALWorldText::ALWorldText():Super() {
	Root = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXT"));
	SetRootComponent(Root);
	// Root->SetFont() // TODO
	// Root->SetMaterial // TODO
	// Root->SetWorldSize() // TODO
}
