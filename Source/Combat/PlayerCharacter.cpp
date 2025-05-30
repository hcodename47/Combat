#include "PlayerCharacter.h"
#include "PlayerCharacterState.h"
#include "AbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerHUD.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void APlayerCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilitySystemComponent();

	InitializeAttributes();
	GiveDefaultAbilities();

	InitHUD();
}

void APlayerCharacter::InitAbilitySystemComponent()
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	AbilityComponent = CastChecked<UAbilitySystemComponent>(PlayerCharacterState->GetAbilitySystemComponent());
	AbilityComponent->InitAbilityActorInfo(PlayerCharacterState, this);
	AttributeSet = PlayerCharacterState->GetAttributeSet();
}

void APlayerCharacter::InitHUD()
{
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
		{
			PlayerHUD->Init();
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue & Value)
{
	if(IsMovementLocked())
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue & Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::SprintStart_Internal(const FInputActionValue & Value)
{
	Sprint(true);
}

void APlayerCharacter::SprintEnd_Internal(const FInputActionValue & Value)
{
	Sprint(false);
}

void APlayerCharacter::CrouchStart_Internal(const FInputActionValue & Value)
{
	CrouchMode(true);
}

void APlayerCharacter::CrouchEnd_Internal(const FInputActionValue & Value)
{
	CrouchMode(false);
}

void APlayerCharacter::AttackAction_Internal(const FInputActionValue & Value)
{
	Attack(false);
}

void APlayerCharacter::HeavyAttackAction_Internal(const FInputActionValue & Value)
{
	Attack(true);
}

void APlayerCharacter::DodgeAction_Internal(const FInputActionValue & Value)
{
	Dodge();
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::SprintStart_Internal);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintEnd_Internal);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::CrouchStart_Internal);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::CrouchEnd_Internal);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::AttackAction_Internal);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::HeavyAttackAction_Internal);

		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::DodgeAction_Internal);
	}
}
