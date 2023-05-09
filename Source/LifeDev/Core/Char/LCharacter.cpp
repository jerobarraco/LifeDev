// Copyright Jerónimo Barraco-Mármol

#include "LCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// ALifeDevCharacter

ALCharacter::ALCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	Camera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ALCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	APlayerController* const PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(Mapping, 0);
}

//////////////////////////////////////////////////////////////////////////// Input

void ALCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input) return;
	//Jumping
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//Moving
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALCharacter::Move);

	//Looking
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALCharacter::Look);
}


void ALCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ALCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
