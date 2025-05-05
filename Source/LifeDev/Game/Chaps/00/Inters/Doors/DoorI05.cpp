// Copyright Jerónimo Barraco-Mármol

#include "DoorI05.h"

ADoorI05::ADoorI05():Super() {
	IsLocked = true;
	UseAnim = false; // will take us directly to the next story step
	UseAutoActivate = true; // leave active since it shows some dialogs.
	IsOneShot = true;
	UseHint = true; // the card is picked as last req
	// no need to unlock. as picking the card will end the chapter 
	// i'm _assuming_ printf is more performant when building strings. because i'd make it like that.
	// ULockCondition = FString::Printf(TEXT("{%ls}&{%ls}&{V.Item.Count.%ls}"),
		// *LDConsts::Flags::Game::CH0::Basin.ToString(),
		// *LDConsts::Flags::Game::CH0::Spot.ToString(),
		// *LDConsts::Items::Card0.ToString());
}

