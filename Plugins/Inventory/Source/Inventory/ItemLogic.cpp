#include "ItemLogic.h"

UItemLogic::UItemLogic():Super() {
	// https://forums.unrealengine.com/t/uobject-begin-play-equivalent/472891/12
	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UItemLogic::BeginPlay);
}
