// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/APCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "APCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/APAnimInstance.h"
#include "Animation/AnimMontage.h"

DEFINE_LOG_CATEGORY(LogAPCharacterPlayer)

AAPCharacterPlayer::AAPCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionJump is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionShoulderMove is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionShoulderLook is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Dash.IA_Dash'"));
	if (nullptr != InputActionDashRef.Object)
	{
		DashAction = InputActionDashRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionDash is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDownRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Down.IA_Down'"));
	if (nullptr != InputActionDownRef.Object)
	{
		DownAction = InputActionDownRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionDown is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLeftRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Left.IA_Left'"));
	if (nullptr != InputActionLeftRef.Object)
	{
		LeftAction = InputActionLeftRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionLeft is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRightRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ARPGPortfolio/Input/Actions/IA_Right.IA_Right'"));
	if (nullptr != InputActionRightRef.Object)
	{
		RightAction = InputActionRightRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("InputActionRight is NULL"));
	}

	CommandTimerHandles.Add(ECommandType::Up, FTimerHandle());
	CommandTimerHandles.Add(ECommandType::Down, FTimerHandle());
	CommandTimerHandles.Add(ECommandType::Left, FTimerHandle());
	CommandTimerHandles.Add(ECommandType::Right, FTimerHandle());

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BackflipMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Backflip.AM_Backflip'"));
	if (BackflipMontageRef.Object)
	{
		BackflipMontage = BackflipMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("BackflipMontage is NULL"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> QuickstepMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ARPGPortfolio/Animation/AM_Quickstep.AM_Quickstep'"));
	if (QuickstepMontageRef.Object)
	{
		QuickstepMontage = QuickstepMontageRef.Object;
	}
	else
	{
		UE_LOG(LogAPCharacterPlayer, Log, TEXT("QuickstepMontage is NULL"));
	}
}

void AAPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
}

void AAPCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::Dash);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AAPCharacterPlayer::StopDash);
	EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressDownCommand);
	EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressLeftCommand);
	EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &AAPCharacterPlayer::PressRightCommand);
}

void AAPCharacterPlayer::ChangeCharacterControl()
{
}

void AAPCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UAPCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	
	CurrentCharacterControlType = NewCharacterControlType;
}

void AAPCharacterPlayer::SetCharacterControlData(const UAPCharacterControlData* CharacterControlData)
{
	AAPCharacterBase::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AAPCharacterPlayer::Jump()
{
	// Normal Jump
	if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Down]) == -1)
	{
		ACharacter::Jump();
	}
	// Backflip
	else 
	{
		ReleaseCommandTimerHandles();
		
		UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance->GetIsFalling() || AnimInstance->GetIsJumping())
		{
			return;
		}
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(BackflipMontage, 1.2f);
		AddMovementInput(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X), -100);
	}
}

void AAPCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AAPCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AAPCharacterPlayer::Dash()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && !AnimInstance->GetIsFalling() && !AnimInstance->GetIsJumping())
	{
		GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_DASH;
		GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_DASH;
	}
}

void AAPCharacterPlayer::StopDash()
{
	UAPAnimInstance* AnimInstance = Cast<UAPAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (AnimInstance->GetIsFalling() || AnimInstance->GetIsJumping())
		{
			GetWorldTimerManager().SetTimer(StopDashTimerHandle, this, &AAPCharacterPlayer::StopDash, FApp::GetDeltaTime(), false);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(StopDashTimerHandle);
			GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION_WALK;
			GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_WALK;
		}
	}
}

void AAPCharacterPlayer::PressDownCommand()
{
	if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Down]) != -1)
	{
		ReleaseCommandTimerHandles();
	}
	GetWorldTimerManager().SetTimer(CommandTimerHandles[ECommandType::Down], this, &AAPCharacterPlayer::ReleaseCommandTimerHandles, 0.25f, false);
}

void AAPCharacterPlayer::PressLeftCommand()
{
	if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Left]) != -1)
	{
		Quickstep(ECommandType::Left);
	}
	GetWorldTimerManager().SetTimer(CommandTimerHandles[ECommandType::Left], this, &AAPCharacterPlayer::ReleaseCommandTimerHandles, 0.2f, false);
}

void AAPCharacterPlayer::PressRightCommand()
{
	if (GetWorldTimerManager().GetTimerRemaining(CommandTimerHandles[ECommandType::Right]) != -1)
	{
		Quickstep(ECommandType::Right);
	}
	GetWorldTimerManager().SetTimer(CommandTimerHandles[ECommandType::Right], this, &AAPCharacterPlayer::ReleaseCommandTimerHandles, 0.2f, false);
}

void AAPCharacterPlayer::ReleaseCommandTimerHandles()
{
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Up]);
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Down]);
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Left]);
	GetWorldTimerManager().ClearTimer(CommandTimerHandles[ECommandType::Right]);

	CommandTimerHandles[ECommandType::Up].Invalidate();
	CommandTimerHandles[ECommandType::Down].Invalidate();
	CommandTimerHandles[ECommandType::Left].Invalidate();
	CommandTimerHandles[ECommandType::Right].Invalidate();
}

void AAPCharacterPlayer::Quickstep(const ECommandType& InCommandType)
{
	ReleaseCommandTimerHandles();

	FName Section;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	switch (InCommandType)
	{
	case ECommandType::Left:
		Section = FName(TEXT("RightQuickstepStart"));
		AnimInstance->Montage_JumpToSection(Section, QuickstepMontage);
		GetCharacterMovement()->AddForce(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y) * -75);
		break;

	case ECommandType::Right:
		Section = FName(TEXT("LeftQuickstepStart"));
		AnimInstance->Montage_JumpToSection(Section, QuickstepMontage);
		GetCharacterMovement()->AddForce(FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y) * 75);
		break;

	default:
		break;
	}

	AnimInstance->Montage_Play(QuickstepMontage, 1.5f);
}