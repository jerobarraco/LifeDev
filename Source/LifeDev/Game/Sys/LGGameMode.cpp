// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "CoreGlobals.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/PostProcessVolume.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "GameFramework/SpectatorPawn.h"

#include "Eval.h"
#include "Interact/CInteract.h"
#include "Inventory/Inventory.h"
#include "Inventory/Flags.h"
#include "Inventory/InventoryMan.h"
#include "Interact/CInteractor.h"
#include "Interact/Animator/CAnimator.h"
#include "Story/StoryMan.h"
#include "Story/Story.h"
#include "Sounds/MusicMan.h"
#include "Diags/Diags.h"
#include "JSig/CSignificance.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "JUtils/Misc/JUtilsSys.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Sentry.h"
#include "LifeDev/Core/Settings/FLChapter.h"
#include "LifeDev/Core/Settings/LFeatsMan.h"
#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Story/LStep.h"
#include "LifeDev/Core/Story/LStoryMan.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Char/LGPController.h"
#include "LifeDev/Game/Dialogs/LDiagMan.h"
#include "LifeDev/Game/Env/Ghost/GhostPool.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Flashback/FlashbackMan.h"
#include "LifeDev/Game/Inventory/LInventoryMan.h"
#include "LifeDev/Game/Inventory/Teach/LTeachMan.h"

DEFINE_LOG_CATEGORY_STATIC(LogLGameMode, Log, Log);

ALGGameMode::ALGGameMode():Super() {
	Super::SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ALChar>
		CChar(TEXT("/Game/LifeDev/Game/Char/LChar_B"));
	DefaultPawnClass = CChar.Succeeded() ? CChar.Class.Get(): ALChar::StaticClass();

	PlayerControllerClass = ALGPController::StaticClass();
	SpectatorClass = ASpectatorPawn::StaticClass();

	// UCInteractor::SetCollisionChannel(InteractTraceChannel);
	// not using the interact trace channel
	// since i want things like walls to block it,
	// without having to set it up for every single mesh.
	// Setting it to visibility has its downsides, like having to set up more complicated collisions
	UCInteractor::SetCollisionChannel(ECC_Visibility);
	UCInteract::SetProfileDefault(UCInteract::ProfileInteract);
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();
	
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	const ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (UNLIKELY(!IsValid(Instance))) {
		UE_LOG(LogLGameMode, Error, TEXT("%hs No valid game instance found! S T O P."), __func__);
		return;
	}

	Settings = Instance->GetSubsystem<ULSettings>();
	if (UNLIKELY(!IsValid(Settings))) {
		UE_LOG(LogLGameMode, Error, TEXT("%hs Settings not valid. can't continue. S T O P."), __func__);
		return;
	}

	// spawn before loading the savegame. in case the Init gets triggered immediately.
	// anything that needs a fully working savegame, needs to happen on init.
	// that's an indirect note to self to ensure all spawned objects don't mess with the savegame now and in the future.
	Spawn();

	// beware of the returns below
	
	// load the save and init. note: the init is blocked until the save is available since we really
	// need that beforehand. and can't work reliably without it.
	// all the important objects are also spawned dynamically and not set in world, that gives us more control.
	// - Thank you so much Jero, that's really how i needed it.
	// - dou itashimashite!
	if (UNLIKELY(!IsValid(Settings->Save))) {
		UE_LOG(LogLGameMode, Log, TEXT("%hs Savegame not valid. Attempt to load or create."), __func__);
		Settings->OnSaving.AddUniqueDynamic(this, &ALGGameMode::InitOnSave);
		Settings->Init(); // force load. if it's currently loading then it won't re-trigger
		return;
	}

	if (UNLIKELY(Settings->GetIsSaving())) {
		Settings->OnSaving.AddUniqueDynamic(this, &ALGGameMode::InitOnSave);
		UE_LOG(LogLGameMode, Log, TEXT("%hs Savegame currently loading. waiting for it."), __func__);
		return;
	}

	UE_LOG(LogLGameMode, Log, TEXT("%hs Savegame seems loaded."), __func__);

	// manually go to init if it's already loaded.
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimerForNextTick(this, &ALGGameMode::Init);
}

void ALGGameMode::InitOnSave(const bool IsSaving) {
	if (LIKELY(!IsSaving)) Init();
}

void ALGGameMode::Spawn() {
	UE_LOG(LogLGameMode, Log, TEXT("%hs."), __func__);

	UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	/// unrelated (done first since other things can depend on this)
	PostProcess = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass()));
	UE_CLOG(UNLIKELY(!PostProcess), LogLGameMode, Error, TEXT("%hs Could not obtain the PostProcess volume."), __func__);
	Char = Cast<ALChar>(UGameplayStatics::GetActorOfClass(World, ALChar::StaticClass()));
	UE_CLOG(UNLIKELY(!Char), LogLGameMode, Error, TEXT("%hs Could not obtain the LCharacter!"), __func__);

	// only one to initialize right here. since i might want to know if some subsystem or actor causes issues
	// during initialization.
	// it does depend on the char. so i do it after obtaining it.
	USentry* const Sentry = USentry::Instance(this);
	if (LIKELY(Sentry)) Sentry->GameInit();

	// spawn these here. since they might take time to actually create.
	// also important since the storyman creates the ui that performs a blank bg. (which probably should be moved to the GameUI in the char)
	InventoryMan = Cast<ALInventoryMan>(World->SpawnActor(ALInventoryMan::StaticClass()));
	StoryMan = Cast<ALStoryMan>(World->SpawnActor(ALStoryMan::StaticClass()));
	DiagMan = Cast<ALDiagMan>(World->SpawnActor(ALDiagMan::StaticClass()));
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));
	FlashbackMan = Cast<AFlashbackMan>(World->SpawnActor(AFlashbackMan::StaticClass()));
	FeatsMan = Cast<ALFeatsMan>(World->SpawnActor(ALFeatsMan::StaticClass()));
	TeachMan = Cast<ALTeachMan>(World->SpawnActor(ALTeachMan::StaticClass()));
	Ghosts = Cast<AGhostPool>(World->SpawnActor(AGhostPool::StaticClass())); // does not need to be here. could be on the featsman
}

void ALGGameMode::Init() {
	// this is the place were we are going to be initializing everything.
	// the savegame should be already loaded.
	Settings->OnSaving.RemoveAll(this);

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (UNLIKELY(!IsValid(SysSettings))) {
		UE_LOG(LogLGameMode, Error, TEXT("%hs System Settings not valid. can't continue."), __func__);
		return;
	}
	
	/// set input mode
	// this is critical or the dialogs will break
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(World, 0);
	// these are not needed since we are using the input actions
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	Controller->bShowMouseCursor = false;

#pragma region Subsystems
	// start by initializing the subsystems, since most other stuff needs it.

	Flashback = World->GetSubsystem<UFlashback>();
	if (UNLIKELY(!Flashback)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Flashback subsystem. Stop."), __func__);
		return;
	}
	
	Diags = World->GetSubsystem<UDiags>();
	if (UNLIKELY(!Diags)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Diags subsystem. Stop."), __func__);
		return;
	}

	/// Inventory
	Flags = World->GetSubsystem<UFlags>();
	if (UNLIKELY(!Flags)){
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Flags subsystem. Stop."), __func__);
		return;
	}
	
	Inventory = World->GetSubsystem<UInventory>();
	if (UNLIKELY(!Inventory)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Inventory subsystem. Stop."), __func__);
		return;
	}
	
	Story = World->GetSubsystem<UStory>();
	if (UNLIKELY(!Story)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Story subsystem. Stop."), __func__);
		return;
	}

	Eval = World->GetSubsystem<UEval>();

	constexpr float FadeTime = 2;
	constexpr float HoldTime = 2;
	const bool IsEditor = UJUtilsSys::IsEditor();
	Story->FadeTime = IsEditor ? 1: FadeTime;
	Story->HoldTime = IsEditor ? 1: HoldTime;

	// init together. but before writing subsystems from save
	Eval->Init();
	UDataTable* InvData = nullptr;
	UDataTable* DiagChars = nullptr;
	UDataTable* DiagDiags = nullptr;
	UDataTable* DiagGroups = nullptr;
	const bool UseExtData = Settings && Settings->GetFeat(EFeat::G_DATA_EXT);
	if (UseExtData) {
		static const FString& Base = FPaths::Combine(FPaths::ProjectConfigDir(), "L10N");
		TArray<FString> Problems;
		UDataTable* const InvExt = UJUtilsMisc::LoadJSONTable(
			Base, SysSettings->Items.GetAssetName(), FItem::StaticStruct(), Problems, this);
		UDataTable* const CharsExt = UJUtilsMisc::LoadJSONTable(Base,
			SysSettings->Characters.GetAssetName(), FDiagChar::StaticStruct(), Problems, this);
		if (LIKELY(CharsExt)) DiagChars = CharsExt; 
		if (LIKELY(InvExt)) InvData = InvExt;
	} else {
		InvData = SysSettings->Items.LoadSynchronous();
		DiagChars = SysSettings->Characters.LoadSynchronous();
		DiagDiags = SysSettings->BaseDiags.LoadSynchronous();
		DiagGroups = SysSettings->BaseGroups.LoadSynchronous();
	}

	Diags->Init();
	Diags->DTCharSet(DiagChars);
	Diags->DTDiagAdd(DiagDiags);
	Diags->DTGroupAdd(DiagGroups);
	Inventory->Init(InvData);
	Flags->Init();
	Story->Init();
	Flashback->Init();
#pragma endregion
	// now load the values from the save
	// ensure the save-game loads the data into the subsystems.
	// do only after subsystems have been initialized.
	// do before StartChapter since that saves the gamefile (loading from subsystems)
	// needs to be forced since sometimes the savegame is already loaded (from the intro level)
	// can be done before the managers since they should load the values anyway. (it's also safer if the values are already there)
	Settings->Save->WriteSubsystems(World);

#pragma region managers
	// set flags, the feats are dependent on the savegame and subsystems
	UCAnimator::Debug = Settings->GetFeat(EFeat::DBG_ANIMS);
	UFlashback::Debug = Settings->GetFeat(EFeat::DBG_FB);
	AStep::Debug = Settings->GetFeat(EFeat::DBG_STEPS);
	UCSignificance::Debug = Settings->GetFeat(EFeat::DBG_SIG);
	
	/// Managers
	// now the managers. which, as they are actors they tend to have side effects,
	// some of which requires the subsystems.
	// some of these tries to use the subsystems on begin play. TODO change that.
	// it's better to spawn these objects before the savegame loads the subsystems or the delegates could confuse them
	// the real solution is to bind to the delegates on init and not beginplay.

	// init together.
	if (LIKELY(IsValid(InventoryMan))) {
		// goes below the dialogs. because some items will trigger a dialog.
		InventoryMan->InputPrio = 9;
		InventoryMan->ZOrder = 1;
		InventoryMan->Init();
	}
	if (LIKELY(IsValid(StoryMan))) {
		StoryMan->ZOrder = 5;
		StoryMan->Init();
	}
	if (LIKELY(IsValid(DiagMan))) {
		// Needs to be 10 so that it takes precedence over the character
		DiagMan->InputPrio = 10;
		DiagMan->ZOrder = 3;
		DiagMan->DebugSkip = !Settings->GetFeat(EFeat::G_SHOW_DIAG); // skip dialogs if no feature for it
		DiagMan->Init();
	}
	if (LIKELY(MusicMan)) MusicMan->Init();
	if (LIKELY(FlashbackMan)) FlashbackMan->Init();
	// do at the end since it depends on other things.
	// will race-condition the ghosts
	if (LIKELY(FeatsMan)) {
		FeatsMan->ZOrder = 111;
		FeatsMan->Init();
	}

	// Character
	if (LIKELY(IsValid(Char))) {
		Char->InputPrio = 1;
		Char->Init();
	}

	if (LIKELY(IsValid(TeachMan))) {
		TeachMan->Init();
		for (const TPair<ETeachTarget, TSoftObjectPtr<UDataTable>>& KV: SysSettings->TeachDTs) {
			TeachMan->AddTarget(KV.Key, KV.Value.LoadSynchronous());
		}
		TeachMan->SetTarget(ETeachTarget::DESK);
	}

	if (LIKELY(Ghosts)) Ghosts->Init();
#pragma endregion

	// start listening only here. in case the previous init might trigger a false one
	Diags->OnShow.AddUniqueDynamic(this, &ALGGameMode::DiagShown);
	Diags->OnDone.AddUniqueDynamic(this, &ALGGameMode::DiagDone);
	Story->OnFade.AddUniqueDynamic(this, &ALGGameMode::Fade);

	// this should be a "safe point" to be loading things, as the story should be showing the black bg
	// Load the base datalayer
	if (UNLIKELY(!SysSettings->BaseDL.IsNull()))
		UJUtilsMisc::ToggleDataLayer(this, SysSettings->BaseDL.LoadSynchronous(), true);
	else
		UE_LOG(LogLGameMode, Warning, TEXT("%hs DLBase not set on the settings! Skip."), __func__);

	FTimerManager& Timer = World->GetTimerManager();
	// force the input disabled. even though the story manager will make this disable later.
	// in case something goes wrong.
	// disable input on next tick to avoid a crash otherwise....
	Timer.SetTimerForNextTick(this, &ALGGameMode::SetInputDisable);

	// finally start
	FTimerHandle Handle;
	// wait for loading. then start the story!
	Timer.SetTimer(Handle, StoryMan.Get(), &ALStoryMan::ChapStart, .1);
	Timer.SetTimer(CounterHandle, this, &ALGGameMode::TickCounter, CounterTime, true);
}

void ALGGameMode::SetCharInputEnabled(const bool Enabled) {
	UE_LOG(LogLGameMode, Log, TEXT("%hs. Enabled=%i"), __func__, Enabled);
	CharInputEnabled = Enabled;
	SetTempInputEnabled(Enabled);
}

void ALGGameMode::SetTempInputEnabled(const bool Enabled) const {
	UE_LOG(LogLGameMode, Log, TEXT("%hs. Enabled=%i"), __func__, Enabled);
	if (UNLIKELY(Enabled && !CharInputEnabled)) return;

	if (LIKELY(IsValid(Char))) Char->SetInputEnabled(Enabled);
	if (LIKELY(IsValid(InventoryMan))) InventoryMan->SetVisible(Enabled);
}

ALGGameMode* ALGGameMode::Instance(const UObject* const O) {
	const UWorld* const World = O? O->GetWorld(): nullptr;
	if (UNLIKELY(!IsValid(World))) return nullptr;

	AGameModeBase* const AuthGameMode = World->GetAuthGameMode();
	if (UNLIKELY(!IsValid(AuthGameMode))) return nullptr;
	
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(AuthGameMode);
	if (UNLIKELY(!IsValid(LGGameMode))) return nullptr;

	return LGGameMode;
}

void ALGGameMode::DeInit() {
	// i should probably start by unbinding everything, then nullifying at the very end
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	// destruction is reverse order than construction. delegates > actors > managers > subsystems

	World->GetTimerManager().ClearAllTimersForObject(this);

	if (LIKELY(IsValid(Story))) Story->OnFade.RemoveAll(this);

	if (LIKELY(IsValid(Char))) Char->DeInit();
	if (LIKELY(IsValid(Ghosts))) Ghosts->Destroy();
	if (LIKELY(IsValid(TeachMan))) TeachMan->DeInit();
	if (LIKELY(IsValid(DiagMan))) DiagMan->DeInit();
	if (LIKELY(IsValid(InventoryMan))) InventoryMan->DeInit();
	if (LIKELY(IsValid(StoryMan))) StoryMan->DeInit();

	// probably won't get a chance to fade since the game mode is ending. but for sake of completion.
	if (LIKELY(IsValid(MusicMan))) MusicMan->Fade(false);

	if (LIKELY(IsValid(Diags))) {
		Diags->OnShow.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
		Diags->DeInit();
	}

	if (LIKELY(IsValid(Inventory))) Inventory->DeInit();
	
	if (LIKELY(IsValid(Flags))) Flags->DeInit();
	
	USentry* const Sentry = USentry::Instance(this);
	if (LIKELY(Sentry)) Sentry->GameDeInit();

	// nullify at end in case someone tries to reference them
	Eval = nullptr;
	Char = nullptr;
	Ghosts = nullptr;
	TeachMan = nullptr;
	DiagMan = nullptr;
	InventoryMan = nullptr;
	StoryMan = nullptr;
	MusicMan = nullptr;
	FlashbackMan = nullptr;
	Diags = nullptr;
	Inventory = nullptr;
	Flags = nullptr;
	Story = nullptr;
	Flashback = nullptr;
	Settings = nullptr; // no deinit. it's a gameinstance subystem
}

void ALGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}

void ALGGameMode::DiagShown(const FDiag& Diag) {
	SetTempInputEnabled(false);
}

void ALGGameMode::DiagDone() {
	SetTempInputEnabled(true);
}

void ALGGameMode::Fade(const bool bIn, const FText& Text) {
	if (!bIn) {
		SetTempInputEnabled(false);
		return;
	}

	// remove the blinds if it's the 1st time.
	if (UNLIKELY(IsFirstFade)) {
		IsFirstFade = false;
		if (LIKELY(StoryMan)) StoryMan->ShowBGSolid(false);
	}

	// fading in requires a timer. since the step notifies when it just starts
	// using the current set times for accuracy.
	const float CurFade = Story->FadeTime;
	const float Wait = CurFade+Story->HoldTime;
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return; // error

	FTimerManager& Time = World->GetTimerManager();
	if (UNLIKELY(IsFirstFade)) { // removes the solid bg if it's necessary
		IsFirstFade = false;
		FTimerHandle Handle1;
		auto Done = [Story=StoryMan] {
			if (LIKELY(Story)) Story->ShowBGSolid(false);
		};
		Time.SetTimer(Handle1, Done, CurFade, false);
	}

	FTimerHandle Handle2;
	Time.SetTimer(Handle2, this, &ALGGameMode::SetInputEnable, Wait, false);
}

void ALGGameMode::TickCounter() const {
	// while these DO work. they now spam the console with 2 "exec commands"
	// GEngine->Exec(nullptr, TEXT("log LogFlags off"));
	// GEngine->Exec(nullptr, TEXT("log LogFlags on"));
	if (LIKELY(Flags)) Flags->Mod(LDConsts::Flags::Stats::Global::Time, CounterTime, false);
}
