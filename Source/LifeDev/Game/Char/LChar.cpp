// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LChar.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
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
#include "Inventory/ItemLogic.h"
#include "Inventory/Flags.h"
#include "JUtils/JMiscUtils.h"

#include "GameUI.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Settings/LSettingsUI.h"
#include "LifeDev/Game/Snd/CLNoiser.h"
#include "LifeDev/Game/Sys/Consts/ConstDlgs.h"
#include "LifeDev/Game/Sys/Consts/ConstFlags.h"

DEFINE_LOG_CATEGORY_STATIC(LogLChar, Log, Log);

ALChar::ALChar(): Super() {
	Super::SetActorTickEnabled(false);

	UCapsuleComponent* const Capsule = GetCapsuleComponent();
	// Set size for collision capsule
	Capsule->InitCapsuleSize(35.f, 75.0f);
	Capsule->SetCapsuleSize(35,75,true);

	UCharacterMovementComponent* const Movement = GetCharacterMovement();
	if (Movement) {
		Movement->MaxWalkSpeed = SpeedMax;
		Movement->MaxWalkSpeedCrouched = SpeedMax/2;
	}

	// Create a CameraComponent
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Capsule);
	// 40 is a biiit below c18, 45 is almost the same.
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 47.f)); // Position the camera
	Camera->bUsePawnControlRotation = true; // needed to be able to loop up

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

void ALChar::SetUIVisible(bool Visible) {
	if (!IsValid(UI)) return;
	UI->SetVisibility(Visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void ALChar::InteractBegin(UCInteract* Comp) {
	if (!IsValid(UI)) return;
	UI->InteractShowPrompt(Comp->Text);
}

void ALChar::InteractEnd(UCInteract* Comp) {
	if (!IsValid(UI)) return;
	UI->InteractHidePrompt();
}

void ALChar::SetInputEnabled(bool Enabled) {
	UI->SetVisibility(Enabled? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	UJMiscUtils::ToggleMapping(this, Mapping, InputPrio, Enabled);
	InteractSetEnabled(Enabled);
	// this is a stub behaviour to disable noises while the player is not actively playing.
	// it just happens to make sense and require little code. to be improved.
	Noiser->SetActive(Enabled, true);
}

// can't remember why i made this into its own function, probably to be able to call from the outside.
void ALChar::InteractSetEnabled(bool Enabled) {
	Interactor->SetEnabled(Enabled);
}

void ALChar::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	UJMiscUtils::ToggleMapping(this, Mapping, InputPrio, true);
	
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
	if (FB) FB->OnChange.AddUniqueDynamic(this, &ALChar::SetFB);

	if (IsValid(Noiser)) Noiser->Activate();
	else UE_LOG(LogTemp, Warning, TEXT("Could not spawn the noiser!"));

	// i can do this because the class defaults are in code. and then can be changed via config.
	// and they get reloaded on game start (travel to game_l).
	// and also the save-game is loaded before a game travel. and doesn't change during game.
	// with your powers combined, it's me! Captain cringy feat!
	UFlags* const Flags = UFlags::Instance(this);
	if (Flags) {
		const float Foxify =
			-.5 + Flags->Get(LDConsts::Flags::Settings::Global::Foxy); // -.5,.5
		const float SpeedMod = SpeedFoxy * Foxify;
		SpeedMin += SpeedMod;
		SpeedMax += SpeedMod;
		UE_LOG(LogLChar, Log,
			TEXT("%hs WalkSpeed foxified. Min=%.4f, Max=%.4f, Mod=%.4f, Foxify=%.4f"),
			__func__, SpeedMin, SpeedMax, SpeedMod, Foxify);
	}
	
	SetFB(0); // update walk speed values.

	// Interactor->OnToggle.AddUniqueDynamic(this, &ALCharacter::InteractToggle);
	Interactor->OnBegin.AddUniqueDynamic(this, &ALChar::InteractBegin);
	Interactor->OnEnd.AddUniqueDynamic(this, &ALChar::InteractEnd);
}

void ALChar::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	const UWorld* const W = GetWorld();
	if (!W) return;

	Inventory = nullptr;
	Diags = nullptr;
	if (IsValid(UI)) {
		UI->RemoveFromParent();
	}
	UI = nullptr;

	if (IsValid(SettingsUI)) {
		SettingsUI->RemoveFromParent();
	}
	SettingsUI = nullptr;

	if (IsValid(Noiser)) Noiser->Deactivate();
	Noiser = nullptr;

	UFlashback* const FB = W->GetSubsystem<UFlashback>();
	if (FB) FB->OnChange.RemoveAll(this);

	UJMiscUtils::ToggleMapping(this, Mapping, InputPrio, false);
	// TODO unbind actions (have to find how to store them)
	Super::EndPlay(EndPlayReason);
}

/// Input

void ALChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input) return;

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
	if (!Controller) return;

	// input is a Vector2D
	const FVector2D& V2D = Value.Get<FVector2D>();
	// done this way to account for player rotation
	const FVector& V3D =
		(GetActorForwardVector()*V2D.Y)
		+ (GetActorRightVector()*V2D.X);
	AddMovementInput(V3D);
}

void ALChar::ActLook(const FInputActionValue& Value) {
	if (!Controller) return;
	// input is a Vector2D
	// not cost. i multiply below.
	FVector2D Vector = Value.Get<FVector2D>();
	if (Interactor->GetInterComp()) Vector *= InteractDrag;
	// add yaw and pitch input to controller
	AddControllerYawInput(Vector.X);
	AddControllerPitchInput(Vector.Y);
}

void ALChar::ActInteract() { // don´t make const. the input system does not like it
	if (!Interactor) return;
	// store before calling TryTrigger. since it might become null afterwards :shrug:
	const UCInteract* const Comp = Interactor->GetInterComp();

	Interactor->TryTrigger(); // this is synchronous
	
	if (IsValid(Comp) && IsValid(UI)) {
		UI->SetPrompt(Comp->Text);
		UE_LOG(LogTemp, Log, TEXT("ALChar ActInteract Text = %s"), *Comp->Text.ToString());
	}
}

bool ALChar::Say(const FName& Name) {
	if (!IsValid(Diags)) return false;
	return Diags->AddId(Name);
}

void ALChar::LookItem(const FName& Name) {
	if (Name.IsNone()) {
		UE_LOG(LogLChar, Log, TEXT("LookItem tried to look at an NONE item."))
		return;
	}

	const FString& SName = *Name.ToString();
	
	FItem Item;
	if (!Inventory->Get(Name, Item)) {
		UE_LOG(LogLChar, Log, TEXT("Can´t find the item name='%s'"), *SName);
		return;
	}
	
	UE_LOG(LogLChar, Log, TEXT("LookItem '%s'. Title='%s' Count=%i, description '%s'."),
		*SName, *Item.Title.ToString(), Item.Count, *Item.Description.ToString());

	// say look at stuff.
	// don't even bother with the non-random.
	// if you want to have a non-random sequence you'd have to add 2 keys.
	// but it's cheaper than asking every time for random and not random.
	const FName& DRName = FName(*(SName + "_Look*"));
	// the isValid is for the add below
	if (!Say(DRName) && IsValid(Diags)) {
		// otherwise compose one
		// show the dialog with the description. this is temporary until i make the ui
        FDialog Diag;
        Diag.Type = EDialogType::SYSTEM;
        Diag.Text = Item.Description;
		// TODO consider changing this to main
        Diag.CharRow = "Sys";
        Diags->AddDiag(Diag);
	}

	// trigger manager look
	if (IsValid(Item.Logic)) {
		Item.Logic->Look();
	}
}

void ALChar::ActItem() {
	if (!IsValid(Inventory)) return;
	const FName& Selected = Inventory->GetSelected();
	UE_LOG(LogLChar, Log, TEXT("ActItem=%s"), *Selected.ToString());

	FItem Item;
	const bool Found = Inventory->GetSelectedItem(Item);
	if (!Found) {
		return;
	}

	if (!Item.Usable) {
		UE_LOG(LogLChar, Log, TEXT("Item not usable"));
		Say(LDConsts::Dlgs::Sys::Item::NotUsable);
		return; // always return if not usable
	}

	if (!Inventory->IsCold(Item)) {
		UE_LOG(LogLChar, Log, TEXT("Item not ready"));
		Say(LDConsts::Dlgs::Sys::Item::NotReady);
		return;
	}

	// this will try trigger the item. i can show dialogs there if i need to.
	// though maybe it would be nice to have something generic as well.
	const EItemUseResult Res = Interactor->TryUseItem(Selected);
	if (Res == EItemUseResult::BAD_HANDLED) {
    	UE_LOG(LogLChar, Log, TEXT("Can't use item with that. But it was handled."));
    	return;
    }

	if (Item.SelfUsable) {
		// notice only checking auto-trigger here. so that i can use an auto trigger with an interact too.
		// (notice this if is separate from the one above)
		UE_LOG(LogLChar, Log, TEXT("Item is self-usable. will attempt now. '%s'."), *Item.Title.ToString());
		if (IsValid(Item.Logic)) {
			Item.Logic->Use();
		}
	} else if (Res != EItemUseResult::SUCCESS) { // notice bad handled above returns.
		const bool isBadTarget = Res == EItemUseResult::BAD_TARGET;
		UE_LOG(LogLChar, Log, TEXT("Can't use item with that. %i '%s' badTarget=%i"), Res, *Item.Title.ToString(), isBadTarget);
		const FName& DlgId = isBadTarget ? 
			LDConsts::Dlgs::Sys::Item::BadTarget :
			LDConsts::Dlgs::Sys::Item::NoTarget;
		Say(DlgId);
		return;
	}

	// mark the item as used, it won't trigger the manager.
	// since we don't want to trigger when is used with an interaction.
	Inventory->Use(Selected);
}

void ALChar::ActItemLook() {
	LookItem( Inventory->GetSelected());
}

// i've added the settings here since the character already deals with the input.
// but honestly it'd be nice to have it somewhere else.
void ALChar::ActMenu() { // no const
	if (!IsValid(SettingsUI)) return;

	// toggle
	if (SettingsUI->IsVisible()) {
		MenuDone();
		return;
	}

	SettingsUI->Show();
}

void ALChar::MenuDone() {
	if (!IsValid(SettingsUI)) return;
	SettingsUI->Hide();
}

void ALChar::SetFB(float Value) {
	UCharacterMovementComponent* const Movement = GetCharacterMovement();
	if (!Movement) return;
	Movement->MaxWalkSpeed = FMath::LerpStable(SpeedMax, SpeedMin, Value);
	Movement->MaxWalkSpeedCrouched = Movement->MaxWalkSpeed/2.0;

	if (Camera)
		Camera->SetFieldOfView(FMath::LerpStable(FOVMin, FOVMax, Value));
}
