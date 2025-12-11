// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LChar.h"

#include "CLCharArm.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h" // for the get object
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "Interact/CInteractor.h"
#include "Inventory/Inventory.h"
#include "Inventory/Flags.h"
#include "JUtils/Input/JUtilsInput.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Sounds/CLNoiser.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

#include "GameUI.h"
#include "CLCharCam.h"
#include "CLCharItems.h"
#include "LifeDev/Game/Env/CLDust.h"

DEFINE_LOG_CATEGORY_STATIC(LogLChar, Log, Log);

ALChar::ALChar() {
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.bStartWithTickEnabled = false;

	UCapsuleComponent* const Capsule = GetCapsuleComponent();
	// Set size for collision capsule
	if (LIKELY(Capsule)) Capsule->InitCapsuleSize(20.f, 65.0f);
	// Capsule->SetCapsuleSize(35,65,true);

	UCharacterMovementComponent* const Movement = GetCharacterMovement();
	if (LIKELY(Movement)) {
		Movement->MaxWalkSpeed = SpeedMax;
		Movement->MaxWalkSpeedCrouched = SpeedMax/2;
	}

	// the arm is mostly to have a smoth rotation lag.
	// important to set the usepawncontrolrotation here and disable on the cam
	Arm = CreateDefaultSubobject<UCLCharArm>(TEXT("Arm"));
	Arm->SetupAttachment(Capsule);
	
	Camera = CreateDefaultSubobject<UCLCharCam>(TEXT("Camera"));
	Camera->SetupAttachment(Arm);
	// 40 is a biiit below c18, 45 is almost the same. it was 47 but i feel it's too tall
	Camera->SetRelativeLocation(FVector(-0.f, 0.f, 40.f)); // Position the camera
	Camera->bUsePawnControlRotation = false;

	Interactor = CreateDefaultSubobject<UCInteractor>(TEXT("Interactor"));
	Interactor->SetupAttachment(Camera);
	// UCInteractor::SetCollisionChannel(InteractTraceChannel);
	// not using the interact trace channel
	// since i want things like walls to block it,
	// without having to set it up for every single mesh.
	// Setting it to visibility has its downsides, like having to set up more complicated collisions
	Interactor->SetChannel(ECC_Visibility);

	Items = CreateDefaultSubobject<UCLCharItems>(TEXT("CharItems"));

	Noiser = CreateDefaultSubobject<UCLNoiser>(TEXT("Noiser"));

	Dust = CreateDefaultSubobject<UCLDust>(TEXT("Dust"));
	// attach to the camera. this might create issues when changing cameras, but let's hope not, if the bounds are big enough it will be ok.
	Dust->SetupAttachment(Camera);
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSfx(TEXT("/Game/LifeDev/Game/Env/Noises/Noises.Noises"));
	Noiser->SFX = CSfx.Object;
	
	// load the ui class here with the class finder.
	// and also all the other default objects
	static ConstructorHelpers::FClassFinder<UUserWidget>
		DefaultUI(TEXT("/Game/LifeDev/Game/Sys/GameUI_W"));
	UIClass = DefaultUI.Succeeded() ? DefaultUI.Class.Get() : UGameUI::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
	 	DefaultMapping(TEXT("/Game/LifeDev/Game/Char/Input/IMC_Char"));
	CtxChar = DefaultMapping.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction>
	 	CActionJump(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Jump"));
	ActionJump = CActionJump.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
	 	CActionLook(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Look"));
	ActionLook = CActionLook.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
	 	CActionMove(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Move"));
	ActionMove = CActionMove.Object;
	// This version does NOT work
	// static ConstructorHelpers::FClassFinder<UInputAction> CActionInteract(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Interact"));
	// ActionInteract = CActionInteract.Class.GetDefaultObject();
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionInteract(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Interact"));
	ActionInteract = CActionInteract.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
		 CMapItem(TEXT("/Inventory/Input/IMC_Inventory"));
	CtxItems = CMapItem.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionItem(TEXT("/Inventory/Input/IA_ItemUse"));
	ActionItemUse = CActionItem.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionItemLook(TEXT("/Inventory/Input/IA_ItemLook"));
	ActionItemLook = CActionItemLook.Object;
}

ALChar* ALChar::Instance(const UObject* const O) {
	ALGGameMode* const Mode = ALGGameMode::Instance(O);
	if (UNLIKELY(!IsValid(Mode))) return nullptr;
	return Mode->Char;
}

void ALChar::SetUIVisible(const bool Visible) {
	if (LIKELY(!IsValid(UI))) return;
	UI->SetVisibility(Visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void ALChar::InteractHover(const bool On, UCInteract* const Comp) {
	UE_LOG(LogLChar, Log, TEXT("%hs On=%i Comp=%s"), __func__, On, *GetNameSafe(Comp));

	if (UNLIKELY(!IsValid(UI))) return;
	
	// will hide the prompt on invalid. which is a nice side effect.
	if (On & LIKELY(IsValid(Comp))) {
		UI->PromptShow(Comp->Text);
	} else {
		UI->PromptHide();
	}
}

void ALChar::SetInputEnabled(const bool Enabled) {
	if (LIKELY(IsValid(UI))) UI->SetVisibility(Enabled? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	UJUtilsInput::ToggleContext(this, CtxChar, InputPrio, Enabled);
	InteractSetActive(Enabled);
	// TODO improve, maybe add a param
	// this is a stub behaviour to disable noises while the player is not actively playing.
	// it just happens to make sense and require little code. to be improved.
	Noiser->SetActive(Enabled, true);

	// Note: by default the Interactor is deactivated. It's all good since the story will enable it.
}

// can't remember why i made this into its own function,
// probably to be able to call from the outside.
void ALChar::InteractSetActive(const bool Enabled) const {
	Interactor->SetActive(Enabled);
}

void ALChar::Init() {
	if (LIKELY(Camera)) Camera->Init();
	
	if (UNLIKELY(!Flags)) return;

	// i can do this because the class defaults are in code. and then can be changed via config.
	// and they get reloaded on game start (travel to game_l).
	// and also the save-game is loaded before a game travel. and doesn't change during game.
	// with your powers combined, it's me! Captain cringy feat!
	const float Foxify =
		-.5 + Flags->Get(LDConsts::Flags::Settings::Global::Foxy); // -.5,.5
	const float SpeedMod = SpeedFoxy * Foxify;
	SpeedMin += SpeedMod;
	SpeedMax += SpeedMod;
	UE_LOG(LogLChar, Log,
		TEXT("%hs WalkSpeed foxified. Min=%.4f, Max=%.4f, Mod=%.4f, Foxy=%.4f"),
		__func__, SpeedMin, SpeedMax, SpeedMod, Foxify);

	if (LIKELY(UI)) UI->Init();

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const UFlashback* const FB = World->GetSubsystem<UFlashback>();
	if (LIKELY(FB)) SetFB(FB->GetVal()); // update walk speed values.

	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) {
		Settings->OnFeatUpdateGameplay.AddUniqueDynamic(this, &ALChar::FeatUp);
		Settings->OnFeatUpdateDebug.AddUniqueDynamic(this, &ALChar::FeatUp);
		FeatUp(EFeat::G_SHOW_POINT, Settings->GetFeat(EFeat::G_SHOW_POINT));
		FeatUp(EFeat::G_STATUS, Settings->GetFeat(EFeat::G_STATUS));
		FeatUp(EFeat::DBG_FLY_CAM, Settings->GetFeat(EFeat::DBG_FLY_CAM));
	}
}

void ALChar::DeInit() {
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) {
		Settings->OnFeatUpdateGameplay.RemoveAll(this);
		Settings->OnFeatUpdateDebug.RemoveAll(this);
	}
}

void ALChar::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	UJUtilsInput::ToggleContext(this, CtxChar, InputPrio, true);
	
	UClass* const Class = UIClass.Get();
	if (LIKELY(IsValid(Class))) {
		UI = CreateWidget<UGameUI>(World, Class);
		if (LIKELY(IsValid(UI))) {
			UI->AddToViewport();
			UI->PromptHide();
			// It is of utmost important that ANY canvas (and or root element) in the widget ui to be have its visibilty
			// set to HitTestInvisible or not hit testable. or it will "eat" the cursor
			// UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController); // doesn't do much. but neat to remember 
		}
	}

	Inventory = World->GetSubsystem<UInventory>();
	Diags = World->GetSubsystem<UDiags>();
	Flags = World->GetSubsystem<UFlags>();
	UFlashback* const FB = World->GetSubsystem<UFlashback>();
	if (LIKELY(FB)) FB->OnChange.AddUniqueDynamic(this, &ALChar::SetFB);

	if (LIKELY(IsValid(Noiser))) Noiser->Activate();
	else UE_LOG(LogLChar, Warning, TEXT("%hs: Could not spawn the noiser!"), __func__);

	Interactor->OnHover.AddUniqueDynamic(this, &ALChar::InteractHover);
}

void ALChar::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	ULSettings* const Settings = ULSettings::Instance(this);
	if (Settings) {
		Settings->OnFeatUpdateGameplay.RemoveAll(this);
		Settings->OnFeatUpdateDebug.RemoveAll(this);
	}

	// unbind input
	UJUtilsInput::ToggleContext(this, CtxChar, InputPrio, false);
	UEnhancedInputComponent* const Input = UJUtilsInput::GetInput(this);
	if (LIKELY(Input)) Input->ClearBindingsForObject(this);
	
	UFlashback* const FB = W->GetSubsystem<UFlashback>();
	if (LIKELY(FB)) FB->OnChange.RemoveAll(this);
	if (LIKELY(IsValid(UI))) UI->RemoveFromParent();
	if (LIKELY(IsValid(Noiser))) Noiser->Deactivate();
	if (LIKELY(IsValid(Interactor))) Interactor->OnHover.RemoveAll(this);

	Inventory = nullptr;
	Diags = nullptr;
	Flags = nullptr;
	UI = nullptr;
	Noiser = nullptr;
	Items = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ALChar::PossessedBy(AController* const NewController) {
	Super::PossessedBy(NewController);

	const APlayerController* const Cont = Cast<APlayerController>(NewController);
	if (UNLIKELY(!Cont)) return;
	
	APlayerCameraManager* const Manager = Cont->PlayerCameraManager.Get();
	if (UNLIKELY(!Manager)) return;

	Manager->ViewPitchMax = ViewPitchMax;
	Manager->ViewPitchMin = -ViewPitchMax;
}

/// Input

void ALChar::SetupPlayerInputComponent(UInputComponent* const PlayerInputComponent) {
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UNLIKELY(!Input)) return;

	// no jumping, i don't like. but leave in case i change my mind. also for docs
	// Input->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	// Input->BindAction(ActionJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ALChar::ActMove);
	Input->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ALChar::ActLook);
	Input->BindAction(ActionInteract, ETriggerEvent::Triggered, this, &ALChar::ActInteract);
	Input->BindAction(ActionItemUse, ETriggerEvent::Triggered, this, &ALChar::ActItem);
	Input->BindAction(ActionItemLook, ETriggerEvent::Triggered, this, &ALChar::ActItemLook);
	// Input->BindAction(ActionMenu, ETriggerEvent::Triggered, this, &ALChar::ActMenu);
}

void ALChar::ActMove(const FInputActionValue& Value) {
	if (UNLIKELY(!Controller)) return;

	// input is a Vector2D
	const FVector2D& V2D = Value.Get<FVector2D>();
	// done this way to account for player rotation
	const FVector& V3D =
		(GetActorForwardVector()*V2D.Y)
		+ (GetActorRightVector()*V2D.X);
	AddMovementInput(V3D);
}

void ALChar::ActLook(const FInputActionValue& Value) {
	if (UNLIKELY(!Controller)) return;
	// input is a Vector2D
	// not cost. i multiply below.
	FVector2D Vector = Value.Get<FVector2D>();
	if (Interactor->GetHoverComp()) Vector *= InteractDrag;
	// add yaw and pitch input to controller
	AddControllerYawInput(Vector.X);
	AddControllerPitchInput(Vector.Y);
}

void ALChar::ActInteract() { // don't make const. the input system does not like it
	if (UNLIKELY(!Interactor)) return;

	// store before calling TryTrigger. since it might become null afterward :shrug:
	const UCInteract* const Comp = Interactor->GetHoverComp();
	Interactor->TryTrigger(); // this is synchronous

	if (IsValid(Comp) & LIKELY(IsValid(UI))) {
		UI->PromptSet(Comp->Text); // update the text with the new state
		UE_LOG(LogLChar, Log, TEXT("%hs Text=%s"), __func__, *Comp->Text.ToString());
	}
}

void ALChar::ActItem() {
	if (LIKELY(IsValid(Items))) Items->UseSelected();
}

void ALChar::ActItemLook() {
	if (LIKELY(IsValid(Items))) Items->LookSelected();
}

void ALChar::SetFB(const float Value) {
	UCharacterMovementComponent* const Movement = GetCharacterMovement();
	if (UNLIKELY(!Movement)) return;

	Movement->MaxWalkSpeed = FMath::LerpStable(SpeedMax, SpeedMin, Value);
	Movement->MaxWalkSpeedCrouched = Movement->MaxWalkSpeed/2.0;
}

void ALChar::FeatUp(const EFeat Feat, const bool Enabled) {
	if (Feat == EFeat::G_SHOW_POINT) {
		if (LIKELY(!IsValid(UI))) return;
		UI->SetPointerShow(Enabled); // flag hides
	} else if (Feat == EFeat::G_STATUS) {
		if (LIKELY(IsValid(UI))) UI->ShowStatus(Enabled);
	} else if (Feat == EFeat::DBG_FLY_CAM) {
		APlayerController* const Cont = UJUtilsInput::GetFirstLocalPlayerController(this);
		if (UNLIKELY(!Cont)) return;

		Cont->ChangeState(Enabled ? NAME_Spectating : NAME_Playing);
		if (!Enabled) Cont->Possess(this); // important since this is not implemented on the controller :/
		// this could be fixed by inheriting from controller and overriding ShouldKeepCurrentPawnUponSpectating

		// still i need a way to access the settings from the spectator.
		// when i go into spectator mode i loose input.
	}
}
