// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMsgBox.h"

#include "UObject/ConstructorHelpers.h"

ULMsgBox::ULMsgBox():Super() {
	static ConstructorHelpers::FClassFinder<UJButton>
		CBtn(TEXT("/Game/LifeDev/Core/UI/Btns/LButtonSmall.LButtonSmall_C"));
	BtnClass = CBtn.Class;
}
