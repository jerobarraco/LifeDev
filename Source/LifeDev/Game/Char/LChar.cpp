// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LChar.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h" // for the get object
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "Interact/CInteractor.h"
#include "Inventory/Inventory.h"
#include "Inventory/Flags.h"
#include "JUtils/Misc/JUtilsSys.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Settings/LSettingsUI.h"
#include "LifeDev/Core/Sounds/CLNoiser.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

#include "GameUI.h"
#include "CLCharCam.h"
#include "CLCharItems.h"

DEFINE_LOG_CATEGORY_STATIC(LogLChar, Log, Log);

ALChar::ALChar(): Super() {
	Super::SetActorTickEnabled(false);

	UCapsuleComponent* const Capsule = GetCapsuleComponent();
	// Set size for collision capsule
	if (LIKELY(Capsule)) Capsule->InitCapsuleSize(20.f, 65.0f);
	// Capsule->SetCapsuleSize(35,65,true);

	UCharacterMovementComponent* const Movement = GetCharacterMovement();
	if (LIKELY(Movement)) {
		Movement->MaxWalkSpeed = SpeedMax;
		Movement->MaxWalkSpeedCrouched = SpeedMax/2;
	}

	// Create a CameraComponent
	Camera = CreateDefaultSubobject<UCLCharCam>(TEXT("Camera"));
	Camera->SetupAttachment(Capsule);
	// 40 is a biiit below c18, 45 is almost the same.
	Camera->SetRelativeLocation(FVector(-0.f, 0.f, 47.f)); // Position the camera

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Interactor = CreateDefaultSubobject<UCInteractor>(TEXT("Interactor"));
	Interactor->SetupAttachment(Camera);

	Items = CreateDefaultSubobject<UCLCharItems>(TEXT("CharItems"));

	Noiser = CreateDefaultSubobject<UCLNoiser>(TEXT("Noiser"));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSfx(TEXT("/Game/LifeDev/Game/Env/Noises/Noises.Noises"));
	Noiser->SFX = CSfx.Object;
	
	// load the ui class here with the class finder.
	// and also all the other default objects
	static ConstructorHelpers::FClassFinder<UUserWidget>
		DefaultUI(TEXT("/Game/LifeDev/Game/Char/W_GameUI"));
	UIClass = DefaultUI.Succeeded() ? DefaultUI.Class.Get() : UGameUI::StaticClass();
	static ConstructorHelpers::FClassFinder<UUserWidget>
		CSettingsUI(TEXT("/Game/LifeDev/Core/Settings/SettingsUI_W"));
	SettingsUIClass = CSettingsUI.Succeeded() ? CSettingsUI.Class.Get() : ULSettingsUI::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
	 	DefaultMapping(TEXT("/Game/LifeDev/Game/Char/Input/IMC_Char"));
	Mapping = DefaultMapping.Object;
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
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionItem(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Item"));
	ActionItem = CActionItem.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionItemLook(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_ItemLook"));
	ActionItemLook = CActionItemLook.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionMenu(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Menu"));
	ActionMenu = CActionMenu.Object;
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

void ALChar::InteractHover(bool bOn, UCInteract* const Comp) {
	if (UNLIKELY(!IsValid(UI))) return;
	// will hide the prompt on invalid. which is a nice side effect. 
	if (bOn && IsValid(Comp))
		UI->InteractShowPrompt(Comp->Text);
	else
		UI->InteractHidePrompt();
}

void ALChar::SetInputEnabled(const bool Enabled) {
	if (LIKELY(IsValid(UI))) UI->SetVisibility(Enabled? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	UJUtilsSys::ToggleMapping(this, Mapping, InputPrio, Enabled);
	InteractSetActive(Enabled);
	// TODO improve, maybe add a param
	// this is a stub behaviour to disable noises while the player is not actively playing.
	// it just happens to make sense and require little code. to be improved.
	Noiser->SetActive(Enabled, true);

	// Note: by default the Interactor is deactivated. It's all good since the story will enable it.
}

// can't remember why i made this into its own function,
// probably to be able to call from the outside.
void ALChar::InteractSetActive(const bool Enabled) {
	Interactor->SetActive(Enabled);
}

void ALChar::Init_Implementation() {
	if (LIKELY(Camera)) Camera->Init();

	// i can do this because the class defaults are in code. and then can be changed via config.
	// and they get reloaded on game start (travel to game_l).
	// and also the save-game is loaded before a game travel. and doesn't change during game.
	// with your powers combined, it's me! Captain cringy feat!
	const UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;

	const float Foxify =
		-.5 + Flags->Get(LDConsts::Flags::Settings::Global::Foxy); // -.5,.5
	const float SpeedMod = SpeedFoxy * Foxify;
	SpeedMin += SpeedMod;
	SpeedMax += SpeedMod;
	UE_LOG(LogLChar, Log,
		TEXT("%hs WalkSpeed foxified. Min=%.4f, Max=%.4f, Mod=%.4f, Foxy=%.4f"),
		__func__, SpeedMin, SpeedMax, SpeedMod, Foxify);

	const UWorld* const World = GetWorld();
	const UFlashback* const FB = World->GetSubsystem<UFlashback>();
	if (LIKELY(FB)) SetFB(FB->GetVal()); // update walk speed values.
}

void ALChar::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	UJUtilsSys::ToggleMapping(this, Mapping, InputPrio, true);
	
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = CreateWidget<UGameUI>(World, Class);
		if (IsValid(UI)) {
			UI->AddToViewport();
			UI->InteractHidePrompt();
			// It is of utmost important that ANY canvas (and or root element) in the widget ui to be have its visibilty
			// set to HitTestInvisible or not hit testable. or it will "eat" the cursor
			// UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController); // doesn't do much. but neat to remember 
		}
	}

	UClass* const SClass = SettingsUIClass.Get();
	if (IsValid(SClass)) {
		SettingsUI = CreateWidget<ULSettingsUI>(World, SClass);
		if (SettingsUI) {
			// important to add to the viewport otherwise the GC will delete our bindings :')
			SettingsUI->AddToViewport(9999);
			SettingsUI->AutoUnbind = false; // you only bind once.
			SettingsUI->OnDone.AddUniqueDynamic(this, &ALChar::MenuDone);
			MenuDone(); // hide
		}
	}

	Inventory = World->GetSubsystem<UInventory>();
	Diags = World->GetSubsystem<UDiags>();
	UFlashback* const FB = World->GetSubsystem<UFlashback>();
	if (LIKELY(FB)) FB->OnChange.AddUniqueDynamic(this, &ALChar::SetFB);

	if (LIKELY(IsValid(Noiser))) Noiser->Activate();
	else UE_LOG(LogLChar, Warning, TEXT("%hs: Could not spawn the noiser!"), __func__);

	Interactor->OnHover.AddUniqueDynamic(this, &ALChar::InteractHover);
}

void ALChar::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	Inventory = nullptr;
	Diags = nullptr;
	if (LIKELY(IsValid(UI))) UI->RemoveFromParent();
	UI = nullptr;

	if (LIKELY(IsValid(SettingsUI))) SettingsUI->RemoveFromParent();
	SettingsUI = nullptr;

	if (LIKELY(IsValid(Noiser))) Noiser->Deactivate();
	Noiser = nullptr;
	Items = nullptr;

	UFlashback* const FB = W->GetSubsystem<UFlashback>();
	if (LIKELY(FB)) FB->OnChange.RemoveAll(this);

	if (LIKELY(IsValid(Interactor))) Interactor->OnHover.RemoveAll(this);

	UJUtilsSys::ToggleMapping(this, Mapping, InputPrio, false);
	UEnhancedInputComponent* const Input = UJUtilsSys::GetEInput(this);
	if (LIKELY(Input)) Input->ClearBindingsForObject(this);

	// TODO unbind actions (have to find how to store them)
	Super::EndPlay(EndPlayReason);
}

/// Input

void ALChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UNLIKELY(!Input)) return;

	// no jumping, i don't like. but leave in case i change my mind. also for docs
	// Input->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	// Input->BindAction(ActionJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ALChar::ActMove);
	Input->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ALChar::ActLook);
	Input->BindAction(ActionInteract, ETriggerEvent::Triggered, this, &ALChar::ActInteract);
	Input->BindAction(ActionItem, ETriggerEvent::Triggered, this, &ALChar::ActItem);
	Input->BindAction(ActionItemLook, ETriggerEvent::Triggered, this, &ALChar::ActItemLook);
	Input->BindAction(ActionMenu, ETriggerEvent::Triggered, this, &ALChar::ActMenu);
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
	// store before calling TryTrigger. since it might become null afterwards :shrug:
	const UCInteract* const Comp = Interactor->GetHoverComp();

	Interactor->TryTrigger(); // this is synchronous
	
	if (IsValid(Comp) && IsValid(UI)) {
		UI->SetPrompt(Comp->Text); // update the text
		UE_LOG(LogLChar, Log, TEXT("%hs Text=%s"), __func__, *Comp->Text.ToString());
	}
}

void ALChar::ActItem() {
	if (LIKELY(IsValid(Items))) Items->UseSelected();
}

void ALChar::ActItemLook() {
	if (LIKELY(IsValid(Items))) Items->LookSelected();
}

// i've added the settings here since the character already deals with the input.
// but honestly it'd be nice to have it somewhere else.
void ALChar::ActMenu() { // no const
	if (UNLIKELY(!IsValid(SettingsUI))) return;

	// toggle
	if (SettingsUI->IsVisible()) {
		MenuDone();
		return;
	}

	SettingsUI->Show();
}

void ALChar::MenuDone() {
	if (UNLIKELY(!IsValid(SettingsUI))) return;
	SettingsUI->Hide();
}

void ALChar::SetFB(const float Value) {
	UCharacterMovementComponent* const Movement = GetCharacterMovement();
	if (UNLIKELY(!Movement)) return;

	Movement->MaxWalkSpeed = FMath::LerpStable(SpeedMax, SpeedMin, Value);
	Movement->MaxWalkSpeedCrouched = Movement->MaxWalkSpeed/2.0;
}