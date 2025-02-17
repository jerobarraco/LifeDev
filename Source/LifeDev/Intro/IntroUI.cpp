// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroUI.h"

#include "MsgBox.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"

UIntroUI::UIntroUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CNewMusic(TEXT("/Game/LifeDev/Game/Env/Music/Music09/Music09_MS.Music09_MS"));
	MusicNew = CNewMusic.Object;
}

void UIntroUI::ShowMsg_Implementation(const FText& Msg) {
	if (UNLIKELY(!MsgBox)) return;
	static TArray<FText> Texts = {
		NSLOCTEXT("Intro", "MsgBox.Btn.OK", "Ok")
	};
	
	MsgBox->SetUp(Msg, Texts);
	MsgBox->Show();
	// this is ok because Hide will unbind.
	// but be careful since this msgbox object is reused for other things
	// like save game erase warning
	MsgBox->OnDone.AddUniqueDynamic(MsgBox, &UMsgBox::Hide);
}

void UIntroUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
}

void UIntroUI::NativeDestruct() {
	Super::NativeDestruct();
}

void UIntroUI::SlotsLoadDone(const bool HasDoneSave) {
	// TODo move this to the gamemode or smth
	ALMusicMan* const Man = ALMusicMan::Instance(this);
	if (UNLIKELY(!Man)) return;
	Man->PlayMusic(MusicNew, true);
}
