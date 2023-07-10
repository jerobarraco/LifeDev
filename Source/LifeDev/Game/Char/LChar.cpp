// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LChar.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h" // for the get object
#include "GameUI.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Dialogs/Dialogs.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interact/CInteract.h"
#include "Interact/CInteractor.h"
#include "Sounds/CNoiser.h"
#include "Inventory/Inventory.h"
#include "JUtils/JMiscUtils.h"

ALChar::ALChar(): Super()
{
	SetActorTickEnabled(false);

	UCapsuleComponent* const Capsule = GetCapsuleComponent();
	// Set size for collision capsule
	Capsule->InitCapsuleSize(35.f, 75.0f);
	Capsule->SetCapsuleSize(35,75,true);

	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();
	if (MovementComponent) {
		MovementComponent->MaxWalkSpeed = 150;
		MovementComponent->MaxWalkSpeedCrouched = 75;
	}
	// Create a CameraComponent	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Capsule);
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 50.f)); // Position the camera
	Camera->bUsePawnControlRotation = true;

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

	Noiser = CreateDefaultSubobject<UCNoiser>(TEXT("Noiser"));
	Noiser->TimeMin = 45;
	Noiser->TimeMax = 150;
	Noiser->HalfAngleWidth = (360.0-90.0)/2.0;
	Noiser->HalfAngleHeight = 40.0;
	Noiser->DistMin = 50;
	Noiser->DistMax = 600;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSfx(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Snd/Noises/Noises.Noises"));
	Noiser->SFX = CSfx.Object;
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CSfxAtt(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Snd/Noises/Noises_Att.Noises_Att"));
	Noiser->Attenuation = CSfxAtt.Object;
	
	UIClass = UGameUI::StaticClass();
	
	// load the ui class here with the class finder.
	// and also all the other default objects
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultUI(TEXT("/Game/LifeDev/Game/Char/W_GameUI"));
	UIClass = DefaultUI.Succeeded() ? DefaultUI.Class.Get() : UGameUI::StaticClass();
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
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, Enabled, GetWorld());
	InteractSetEnabled(Enabled);
	// this is a stub behaviour to disable noises while the player is not actively playing.
	// it just happens to make sense and require little code. to be improved.
	Noiser->SetIsPlaying(Enabled);
}

// can't remember why i made this into its own function, probably to be able to call from the outside.
void ALChar::InteractSetEnabled(bool Enabled) {
	Interactor->SetEnabled(Enabled);
}

void ALChar::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, true, World);
	
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UGameUI>(this, Class);
		UI->AddToViewport();
		UI->InteractHidePrompt();
		// It is of utmost important that ANY canvas (and or root element) in the widget ui to be have its visibilty
		// set to HitTestInvisible or not hit testable. or it will "eat" the cursor
		// UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController); // doesn't do much. but neat to remember 
	}
	// Interactor->OnToggle.AddUniqueDynamic(this, &ALCharacter::InteractToggle);
	Interactor->OnBegin.AddUniqueDynamic(this, &ALChar::InteractBegin);
	Interactor->OnEnd.AddUniqueDynamic(this, &ALChar::InteractEnd);
	Inventory = World->GetSubsystem<UInventory>();

	Noiser->Start();
}

void ALChar::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(UI)) {
		UI->RemoveFromParent();
	}
	UI = nullptr;
	Inventory = nullptr;
	if (IsValid(Noiser)) {
		Noiser->Stop();
	}
	Noiser = nullptr;

	UJMiscUtils::ToggleMapping(Mapping, InputPrio, false, GetWorld());
	// TODO unbind actions
	Super::EndPlay(EndPlayReason);
}

/// Input

void ALChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input) return;

	Input->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	Input->BindAction(ActionJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ALChar::ActMove);
	Input->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ALChar::ActLook);
	Input->BindAction(ActionInteract, ETriggerEvent::Triggered, this, &ALChar::ActInteract);
	Input->BindAction(ActionItem, ETriggerEvent::Triggered, this, &ALChar::ActItem);
	Input->BindAction(ActionItemLook, ETriggerEvent::Triggered, this, &ALChar::ActItemLook);
}

void ALChar::ActMove(const FInputActionValue& Value)
{
	if (!Controller) return;

	// input is a Vector2D
	const FVector2D& MovementVector = Value.Get<FVector2D>();
	// add movement 
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ALChar::ActLook(const FInputActionValue& Value)
{
	if (!Controller) return;
	// input is a Vector2D
	FVector2D Vector = Value.Get<FVector2D>();
	if (Interactor->GetInterComp()) {
		Vector *= InteractDrag;
	}
	// add yaw and pitch input to controller
	AddControllerYawInput(Vector.X);
	AddControllerPitchInput(Vector.Y);
}

void ALChar::ActInteract(const FInputActionValue& Value) {
	if (!Interactor) return;
	Interactor->TryTrigger();
	const UCInteract* const Comp = Interactor->GetInterComp();
	if (IsValid(Comp) && IsValid(UI)) {
		UI->SetPrompt(Comp->Text);
	}
}

void ALChar::LookItem(const FItem& Item) {
	// TODO open the ui and show it
	UE_LOG(LogTemp, Warning, TEXT(
				"This is a erzats display for the item '%s'. Look how beautiful it is!"
				"you have %i of them, and the description is : '%s'. "),
			*Item.Title.ToString(), Item.Count, *Item.Description.ToString());

	// show the dialog with the description. this is temporary until i make the ui
	FDialog Diag;
	Diag.Type = EDialogType::SYSTEM;
	Diag.Text = Item.Description;
	Diag.CharRow = "Main"; 
	UDialogs* const D = GetWorld()->GetSubsystem<UDialogs>();
	if (!D) return;
	D->AddDiag(Diag);

	// Say(FName("IT_NotUsable"))
	// FDialogChar Char;
	// D->AddId(FName("ItemNotUsable"), Diag, Char);
}

bool ALChar::Say(const FName& Name) {
	UDialogs* const D = GetWorld()->GetSubsystem<UDialogs>();
	if (!D) return false;
	FDialog Diag; FDialogChar Char;
	return D->AddId(Name);
}

void ALChar::ActItem() {
	if (!IsValid(Inventory)) return;
	const FName& Selected = Inventory->GetSelected();
	UE_LOG(LogTemp, Log, TEXT("ActItem=%s"), *Selected.ToString());

	FItem Item;
	const bool Found = Inventory->GetSelectedItem(Item);
	if (!Found) {
		return;
	}

	if (!Item.Usable) {
		LookItem(Item);
		return;
	}

	if (!Inventory->IsCold(Item)) {
		UE_LOG(LogTemp, Warning, TEXT("Can't use item."));
		if (Say(FName("IT_NotReady"))) return;
		// TODO show text
		return;
	}

	// this will try trigger the item. i can show dialogs there if i need to.
	// though maybe it would be nice to have something generic as well.
	EItemUseResult Res = Interactor->TryUseItem(Selected);
	if (Res != EItemUseResult::SUCCESS){
		// notice only checking auto-trigger here. so that i can use an auto trigger with an interact too.
		if (Item.SelfUsable) {
			// TODO trigger effect here
			UE_LOG(LogTemp, Warning, TEXT("Stub effect trigger for item '%s'."), *Item.Title.ToString());
		} if (Res == EItemUseResult::BAD_HANDLED) {
			UE_LOG(LogTemp, Log, TEXT("Can't use item with that. But it was handled."));
			return;
		} else {
			// the issue is that the item itself will be displaying a text. and i can't tell if there is no item
			UE_LOG(LogTemp, Log, TEXT("Can't use item with that."));
			Say(Res == EItemUseResult::BAD_TARGET ? FName("IT_BadTarget") : FName("IT_NoTarget"));
			return;
		} 
	}

	Inventory->Use(Selected);
}

void ALChar::ActItemLook() {
	FItem Item;
	Inventory->GetSelectedItem(Item);
	LookItem(Item);
}
