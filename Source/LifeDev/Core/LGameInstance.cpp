// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LGameInstance.h"

#include "MoviePlayer.h"

ULGameInstance* ULGameInstance::Get(UWorld* World) {
	if (!IsValid(World)) return nullptr;
	return Cast<ULGameInstance>(World->GetGameInstance());
}

bool ULGameInstance::GetFeatS(UWorld* World, EFeat Feat) {
	ULGameInstance* const I = ULGameInstance::Get(World);
	return IsValid(I)? I->GetFeat(Feat) : false;
}

void ULGameInstance::Init() {
	Super::Init();

	// https://unrealcommunity.wiki/loading-screen-243mzpq1
	// create widget https://forums.unrealengine.com/t/createwidget-c/462559/2
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULGameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULGameInstance::EndLoadingScreen);
	ResetFeats();
}

void ULGameInstance::SetTrs(bool Enabled) {
	// this is just test code... 
	GetWorld()->Exec(GetWorld(), TEXT("r.AntiAliasingMethod 2"));
	GetWorld()->Exec(GetWorld(), TEXT("r.test.SecondaryScaleOverride 4"));
	GetWorld()->Exec(GetWorld(), TEXT("t.MaxFPS 60"));
	GetWorld()->Exec(GetWorld(), TEXT("r.Upscale.Quality 1")); // Simple bilinear https://docs.unrealengine.com/4.27/en-US/RenderingAndGraphics/ScreenPercentage/
	GetWorld()->Exec(GetWorld(), TEXT("r.SecondaryScreenPercentage.GameViewport 50"));
	GetWorld()->Exec(GetWorld(), TEXT("sg.ResolutionQuality 40"));
	GetWorld()->Exec(GetWorld(), TEXT("r.TemporalAA.Upsampling 0"));
	GetWorld()->Exec(GetWorld(), TEXT("r.ScreenPercentage 50"));
	// TODO disable temporal upsampling
	// https://forums.unrealengine.com/t/ue5p2-r-screenpercentage-not-working-for-me/509965/11?u=nande
}


void ULGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (IsRunningDedicatedServer()) return;
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Screen loading is on"));	
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	// use widget->takewidget() to get the swidget
	// https://forums.unrealengine.com/t/how-do-you-convert-a-uuserwidget-to-a-swidget-needed-for-a-loadingscreen/343494/8
	LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

	GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->StartCameraFade(0, 1, .5f, FLinearColor::Black, true, true);
}

void ULGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Screen loading is done"));	

	GetFirstLocalPlayerController(InLoadedWorld)->PlayerCameraManager->StartCameraFade(1, 0, .5f, FLinearColor::Black, true, true);

	// ?
}

void ULGameInstance::ResetFeats() {
	ULSysSettings* const Settings = ULSysSettings::Get();
	 
	#if (UE_BUILD_TEST || UE_BUILD_SHIPPING)
		constexpr bool UseDebug = false;
	#else
		const bool UseDebug = Settings->UseDebugFeats;
	#endif
	
	Feats = UseDebug ? Settings->DebugFeats : Settings->DefaultFeats;
}

bool ULGameInstance::GetFeat(EFeat Feat) const {
	return Feats.Contains(Feat);	
}

void ULGameInstance::SetFeat(EFeat Feat, bool Enable) {
	const bool Has = Feats.Contains(Feat);
	if (Enable) {
		if (!Has) Feats.Add(Feat);
	} else {
		if (Has) Feats.Remove(Feat);
	}
}

// disable lumen on runtime https://forums.unrealengine.com/t/is-there-a-way-to-add-an-option-to-enable-disable-lumen-for-in-game-settings/613756

/*

void USGEGameInstance::LoadGame()
{
	// Try to load a saved game file (with name: <SaveGameSlotName>.sav) if exists
	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
	SaveGameObject = Cast<USGESaveGame>(LoadedGame);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Trying to load a saved game."));
    
	// If file does not exist try create a new one
	if (!SaveGameObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No saved games found. Trying to save a new one."));
        
		// Instantiate a new SaveGame object
		SaveGameObject = Cast<USGESaveGame>(UGameplayStatics::CreateSaveGameObject(USGESaveGame::StaticClass()));

		// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
		const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

		LogIfGameWasSavedOrNot(IsSaved);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saved game found. Loaded."));
	}
    
}

SaveGame method:

void USGEGameInstance::SaveGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saving game..."));
    
	// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
	const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

	LogIfGameWasSavedOrNot(IsSaved);
    
}
*/

